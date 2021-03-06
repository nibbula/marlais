/*
** bytestring.c
*/

#include <string.h>

#include "bytestring.h"

#include "alloc.h"
#include "globaldefs.h"
#include "character.h"
#include "error.h"
#include "number.h"
#include "prim.h"
#include "symbol.h"
#include "sequence.h"

/* primitives */

static Object string_element (Object string, Object index, Object default_ob);
static Object string_element_setter (Object string, Object index, Object val);
static Object string_size (Object string);
static Object string_size_setter (Object size, Object string);
static Object string_append2 (Object str1, Object str2);
static Object string_lessthan (Object str1, Object str2);
static Object string_equal (Object str1, Object str2);

static struct primitive string_prims[] =
{
    {"%string-element", prim_3, string_element},
    {"%string-element-setter", prim_3, string_element_setter},
    {"%string-size", prim_1, string_size},
    {"%string-size-setter", prim_2, string_size_setter},
    {"%string-append2", prim_2, string_append2},
    {"%string<", prim_2, string_lessthan},
    {"%string=", prim_2, string_equal},
};

/* function definitions */

void
init_string_prims (void)
{
    int num = sizeof (string_prims) / sizeof (struct primitive);
    init_prims (num, string_prims);
}

Object
make_byte_string (char *str)
{
    Object obj = allocate_object (sizeof (struct byte_string));

    BYTESTRTYPE (obj) = ByteString;
    BYTESTRSIZE (obj) = strlen (str);
    BYTESTRVAL (obj) = checking_strdup (str);
    return (obj);
}

Object
make_string_driver (Object args)
{
  int size, i;
  char fill;
  Object size_obj, fill_obj, res;

  make_sequence_driver(args, &size, &size_obj, &fill_obj, "<string>");

  if (fill_obj != false_object) {
    if (!CHARP (fill_obj)) {
      error ("make: value of fill: must be a character for <string> class", 
	     fill_obj, NULL);
    }
    fill = CHARVAL (fill_obj);
  } else {
    fill = ' ';
  }

  /* actually fabricate the string */
  res = allocate_object (sizeof (struct byte_string));

  BYTESTRTYPE (res) = ByteString;
  BYTESTRSIZE (res) = size;
  BYTESTRVAL (res) = (char *) checking_malloc ((size * sizeof (char)) + 1);

  for (i = 0; i < size; ++i) {
    BYTESTRVAL (res)[i] = fill;
  }
  BYTESTRVAL (res)[i] = '\0';
  return (res);
}

/* primitives */

static Object
string_element (Object string, Object index, Object default_ob)
{
    int i;

    i = INTVAL (index);
    if ((i < 0) || (i >= BYTESTRSIZE (string))) {
	if (default_ob == default_object) {
	    error ("element: argument out of range", string, index, NULL);
	} else {
	    return default_ob;
	}
    }
    return (make_character (BYTESTRVAL (string)[i]));
}

static Object
string_element_setter (Object string, Object index, Object val)
{
    int i;

    i = INTVAL (index);
    if ((i < 0) || (i >= BYTESTRSIZE (string))) {
	error ("element-setter: argument out of range", string, index, NULL);
    }
    BYTESTRVAL (string)[i] = CHARVAL (val);
    return (unspecified_object);
}

static Object
string_size (Object string)
{
    return (make_integer (BYTESTRSIZE (string)));
}

static Object
string_size_setter (Object size, Object string)
{
    int new_size = INTVAL (size);

    if ((new_size < 0) || (new_size >= BYTESTRSIZE (string))) {
	error ("size-setter: new size out of range", new_size, string, NULL);
    }
    BYTESTRSIZE (string) = new_size;
    BYTESTRVAL (string)[new_size] = '\0';
    return size;
}

static Object
string_append2 (Object str1, Object str2)
{
    char *new_str;
    int new_size;

    new_size = BYTESTRSIZE (str1) + BYTESTRSIZE (str2);
    new_str = (char *) checking_malloc ((new_size * sizeof (char)) + 1);

    strcpy (new_str, BYTESTRVAL (str1));
    strcat (new_str, BYTESTRVAL (str2));
    return (make_byte_string (new_str));
}

static Object
string_lessthan (Object str1, Object str2)
{
    return (strcmp (BYTESTRVAL (str1), BYTESTRVAL (str2)) < 0) ?
	true_object : false_object;
}

static Object
string_equal (Object str1, Object str2)
{
    return (strcmp (BYTESTRVAL (str1), BYTESTRVAL (str2)) == 0) ?
	true_object : false_object;
}
