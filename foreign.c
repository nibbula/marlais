/*
**  foreign.c - foreign function interface
*/

#include <ffi.h>

#include "foreign.h"
#include "prim.h"
#include "error.h"
#include "foreign_ptr.h"
#include "alloc.h"
#include "class.h"
#include "number.h"
#include "globaldefs.h"
#include "bytestring.h" 
#include "character.h"

static Object load_foreign (Object name);
static Object get_foreign (Object file_handle, Object name,
			   Object arg_types, Object return_type);
static Object call_foreign (Object ptr, Object args);

/* Required integer type classes */
static Object c_int_class;
static Object c_signed_int_class;
static Object c_unsigned_int_class;

static Object c_long_class;
static Object c_signed_long_class;
static Object c_unsigned_long_class;

static Object c_short_class;
static Object c_signed_short_class;
static Object c_unsigned_short_class;

static Object c_char_class;
static Object c_signed_char_class;
static Object c_unsigned_char_class;

static void note_c_classes();
static Object find_c_class(char * n);

static struct primitive foreign_prims[] =
{
    {"%load-foreign", prim_1, load_foreign},
    {"%get-foreign", prim_4, get_foreign},
    {"%call-foreign", prim_1_rest, call_foreign},
};

typedef struct {
  void * arg;			/* Incoming argument values */
  Object obj;			/* Value to modify */
} FF_arg;

typedef struct {
  void * file;			/* Handle returned by dlopen() */
  char * name;			/* Name of the function passed to dlsym() */
  void * function;		/* Function to call */
  int argc;			/* Number of arguments */
  Object return_type;
  ffi_cif cif;			/* Info for ffi_call() */
  FF_arg * arg;			/* Array of argument value data */
  void ** arg_list;		/* For passing to ffi_call */
} FF_record;

void
init_foreign_prims (void)
{
    int num;

    num = sizeof (foreign_prims) / sizeof (struct primitive);

    init_prims (num, foreign_prims);
}

#ifdef HAS_DLOPEN

#include <dlfcn.h>

static Object
load_foreign (Object name)
{
    char * filename;
    void * handle;

    filename = BYTESTRVAL (name);
    handle = dlopen (filename, RTLD_LAZY);
    if (!handle) /* Should we use dlerror() ? */
	return error ("%load-foreign: could not load file ", name, NULL);

    return (make_foreign_ptr (handle));
}

/*
** Translate a Dylan object type into an FFI type.
*/
ffi_type *
builtin_to_ffi_type(Object o)
{
  /* Handle some of the built-in types */
  switch (TYPE(o))
  {
    case Integer:
      return &ffi_type_sint;
    case SingleFloat:
      return &ffi_type_float;
    case DoubleFloat:
      return &ffi_type_double;
    case ByteString:
    case Symbol:
    case Keyword:
    case ForeignPtr:
      return &ffi_type_pointer;
    case Character:
      return &ffi_type_schar;
    case False:
      return &ffi_type_void;
    case Class:
      {
	if (o == number_class ||
	    o == small_integer_class ||
	    o == integer_class ||
	    o == c_int_class ||
	    o == c_signed_int_class)		return &ffi_type_sint;
	else if (o == c_unsigned_int_class)	return &ffi_type_uint;
	else if (o == c_short_class ||
		 o == c_signed_short_class)	return &ffi_type_sshort;
	else if (o == c_long_class ||
		 o == c_signed_long_class)	return &ffi_type_slong;
	else if (o == c_unsigned_long_class)	return &ffi_type_ulong;
	else if (o == false_object)		return &ffi_type_void;
	else if (o == float_class ||
		 o == single_float_class)	return &ffi_type_float;
	else if (o == double_float_class)	return &ffi_type_double;
	else if (o == foreign_pointer_class ||
		 o == string_class ||
		 o == symbol_class ||
		 o == keyword_class ||
		 o == byte_string_class)	return &ffi_type_pointer;
	else if (o == character_class ||
	         o == c_char_class ||
	         o == c_signed_char_class)	return &ffi_type_schar;
	else if (o == c_unsigned_char_class)	return &ffi_type_uchar;
	else
	  return 0;
      }
    default:
      return 0;
  }
  return 0;
}

/*
** Create a foreign function description and return a pointer to it that can
** be given later to %call-foreign.
**
** @@@ Perhaps we should remove the type checking of arguments to this
** primative once things are working.
*/
static Object
get_foreign (Object file_handle_obj, Object name_obj,
	     Object arg_types, Object return_type_obj)
{
  FF_record * ff;
  ffi_type ** ff_arg_types;
  ffi_type * ff_result_type;
  int i;
  Object a;
  ffi_status status;

  ff = checking_malloc(sizeof(FF_record));

  /*
  ** Set the file handle
  */
  if (FOREIGNP(file_handle_obj))
    ff->file = FOREIGNPTR(file_handle_obj);
  else
    return error("%get-foreign: first arg is not a pointer: ",
		 file_handle_obj, NULL);

  /*
  ** Set the name
  */
  if (SYMBOLP(name_obj))
  {
    ff->name = SYMBOLNAME(name_obj);
    /* XXX Prepend an underscore. According to the documentation for dlsym
    ** this may not be necessary.  Perhaps we should try first without the
    ** underscore and the add one if the dlsym fails?
    */
    ff->name = checking_malloc(strlen(ff->name) + 1);
    ff->name[0] = '_';
    strcpy(ff->name, SYMBOLNAME(name_obj));
  }
  else if (KEYWORDP(name_obj))
  {
    /* XXX This should be unnecessary if we get rid of the keyword/symbol
    ** storage distinction.
    */
    ff->name = checking_strdup(KEYNAME(name_obj));
    /* "name:" -> "_name" */
    memmove(ff->name + 1, ff->name, strlen(ff->name) - 1);
    ff->name[0] = '_';
  }
  else
    return error("%get-foreign: second arg is not a name: ", name_obj, NULL);

  if (!LISTP(arg_types))
    return error("%get-foreign: third arg is not a list: ", arg_types, NULL);

  /*
  ** Get the actual address
  */
  ff->function = dlsym(ff->file, ff->name);
  if (!ff->function)
    return error("%get-foreign: can't find the symbol: ", name_obj, NULL);

  ff->return_type = return_type_obj;

  /*
  ** Allocate the argument lists
  */
  ff->argc = list_length(arg_types);
  if (ff->argc)
  {
    ff_arg_types = checking_malloc(ff->argc * sizeof(ffi_type));

    /* Preallocate the values area and list for calling later */
    ff->arg = checking_malloc(ff->argc * sizeof(FF_arg));
    ff->arg_list = checking_malloc(ff->argc * sizeof(void *));
  }
  else
  {
    ff_arg_types = 0;
    ff->arg = 0;
    ff->arg_list = 0;
  }

  /*
  ** Set up the argument types
  */
  note_c_classes();
  for (i = 0, a = arg_types; PAIRP(a); ++i, a = CDR(a))
  {
    ff_arg_types[i] = builtin_to_ffi_type(CAR(a));
    if (!ff_arg_types[i])
      return error("Type not supported for foreign functions: ",
		   objectclass(CAR(a)));
  }

  ff_result_type = builtin_to_ffi_type(return_type_obj);
  if (!ff_result_type)
    return error("Type not supported for foreign functions: ",
		 objectclass(return_type_obj));

  /*
  ** Set up the call info
  */
  status = ffi_prep_cif(&ff->cif, FFI_DEFAULT_ABI, ff->argc,
			ff_result_type, ff_arg_types);
  if (status != FFI_OK)
    return error("Couldn't prepare foreign function. "
		 "A type is probably bad.", NULL);

  return make_foreign_ptr((void *)ff);
}

/*
** Call a foreign function.
*/
static Object
call_foreign (Object ptr, Object args)
{
  Object a;
  Object o;
  int ac = 0;
  typedef (*func_ptr)();
  union u {
    char * cp;
    void * vp;
    func_ptr fp;
    long long int lli;
    long int li;
    char c;
    int i;
    float f;
    double d;
    long double ld;
  } result;
  void * result_ptr = &result;

  FF_record * ff;
  int i;
  int len;

  if (!FOREIGNP(ptr) || !FOREIGNPTR(ptr))
    return error("Foreign function fatal fubar.", NULL);

  ff = (FF_record *) FOREIGNPTR(ptr);

  len = list_length(args);
  if (len)
  {
    if (len != ff->argc)
      error("%call-foreign: Number of arguments provided don't "
	    "match description.", NULL);

    for (i = 0, a = args; PAIRP(a); ++i, a = CDR(a))
    {
      o = CAR(a);
      ff->arg[i].obj = o;
      switch (TYPE(o))
      {
	case Integer:
	  ff->arg[i].arg = (void *)INTVAL(o);
	  ff->arg_list[i] = &(ff->arg[i].arg);
	  break;
#if 0 // @@@ dont do floats yet
	case SingleFloat:
	  ff->arg[i].arg = (void *)SFLOATVAL(o);
	  ff->arg_list[i] = &(ff->arg[i].arg);
	  break;
	case DoubleFloat:
	  ff->arg[i].arg = (void *)DFLOATVAL(o);
	  ff->arg_list[i] = &(ff->arg[i].arg);
	  break;
#endif

	case ByteString:
	  /* @@@ What about null termination? */
	  ff->arg[i].arg = (void *)BYTESTRVAL(o);
	  ff->arg_list[i] = &(ff->arg[i].arg);
	  break;
	case Symbol:
	  /* @@@ This is read-only! */
	  ff->arg[i].arg = (void *)SYMBOLNAME(o);
	  ff->arg_list[i] = &(ff->arg[i].arg);
	  break;
	case Keyword:
	  /* @@@ This is read-only! */
	  ff->arg[i].arg = (void *)KEYNAME(o);
	  ff->arg_list[i] = &(ff->arg[i].arg);
	  break;
	case ForeignPtr:
	  ff->arg[i].arg = (void *)FOREIGNPTR(o);
	  ff->arg_list[i] = &(ff->arg[i].arg);
	  break;
	case Character:
	  ff->arg[i].arg = (void *)((int)CHARVAL(o));
	  ff->arg_list[i] = &(ff->arg[i].arg);
	  break;
	default:
	  return error("Unsupported argument type for foreign function: ",
		       ff->name, o, objectclass(o), NULL);
      }
    }
  }

  ffi_call(&ff->cif, FFI_FN(ff->function), result_ptr, ff->arg_list);

  /* TODO: Turn changes to the arguments into extra return values.  This
  ** probably require the ability to specify if an arguement is an "output"
  ** argument.
  */
#if 0 /* @@@ Ignore what happend to the args for now */
  for (i = 0; i < argc; ++i)
  {
  }
#endif

  if (!CLASSP(ff->return_type) && !FALSEP(ff->return_type))
    return error("Return type for foreign function is not a class : ",
		 ff->name, ff->return_type,
		 objectclass(ff->return_type), NULL);

  /*
  ** Translate the return value
  **
  ** XXX Many of these are wrong or a least should signal errors.
  */
  o = ff->return_type;
  if (o == number_class ||	
      o == small_integer_class ||
      o == integer_class ||
      o == c_int_class ||
      o == c_signed_int_class ||
      o == c_unsigned_int_class ||
      o == c_short_class ||
      o == c_signed_short_class ||
      o == c_long_class ||
      o == c_signed_long_class ||
      o == c_unsigned_long_class)	a = make_integer(result.i);
  else if (o == false_object)		a = false_object;
  else if (o == float_class ||
	   o == single_float_class)	a = make_dfloat((double)result.f);
  else if (o == double_float_class)	a = make_dfloat(result.d);
  else if (o == foreign_pointer_class)	a = make_foreign_ptr(result.vp);
  else if (o == string_class ||
	   o == byte_string_class)	a = make_byte_string(result.cp);
  else if (o == symbol_class)		a = make_symbol(result.cp);
  else if (o == keyword_class)		a = make_symbol(result.cp);
  else if (o == character_class ||
	   o == c_char_class ||
	   o == c_signed_char_class ||
	   o == c_unsigned_char_class)
    a = make_character(result.c);
  else
    return error("Unsupported return type for foreign function: ",
		 ff->name, ff->return_type, o, NULL);

  return a;
}

static Object
find_c_class(char * n)
{
  Object o;

  o = symbol_value(make_symbol(n));
  if (!CLASSP(o))
  {
    error("C-FFI Can't find a required class ", make_symbol(n), NULL);
    return object_class;	/* XXX */
  }
  return o;
}

/*
** Get pointers to C-FFI classes.
** This allows the classes to be defined in Dylan rather than C.
*/
static void
note_c_classes()
{
  static int already_noted = 0;

  if (!already_noted)
  {
    c_int_class			= find_c_class("<c-int>");
    c_signed_int_class		= find_c_class("<c-signed-int>");
    c_unsigned_int_class	= find_c_class("<c-unsigned-int>");

    c_long_class		= find_c_class("<c-long>");
    c_signed_long_class		= find_c_class("<c-signed-long>");
    c_unsigned_long_class	= find_c_class("<c-unsigned-long>");

    c_short_class		= find_c_class("<c-short>");
    c_signed_short_class	= find_c_class("<c-signed-short>");
    c_unsigned_short_class	= find_c_class("<c-unsigned-short>");

    c_char_class		= find_c_class("<c-char>");
    c_signed_char_class		= find_c_class("<c-signed-char>");
    c_unsigned_char_class	= find_c_class("<c-unsigned-char>");
  }
  already_noted = 1;
}

#endif /* HAS_DLOPEN */

#ifdef powerc
// load a function from a container with code fragment manager.

static Object
load_foreign (Object name)
{
    char * filename;
    void * handle;

    return error ("load-foreign: not implemented yet ", name, NULL);

#if 0
    filename = BYTESTRVAL (name);
    handle = dlopen (filename, RTLD_LAZY);
    if (!handle) {
    }
    return (make_integer ((int) handle));
#endif
}

#endif /* powerc */
