Module:   dylan
Author:   Nibby Nebbulous
Synopsis: Patheticly impoverished foreign function interface.

// Portions stolen from Gwydion's c-ffi.dylan

define constant <machine-word> = <integer>;

//  Primitive Designator Types
//
//  These are the top-level designator types. None of are instantiable,
//  and none of them have input or output mappings.

define sealed abstract class <C-value> (<object>)
end class <C-value>;

define sealed abstract class <C-void> (<C-value>)
end class <C-void>;

define sealed abstract class <C-number> (<C-value>)
end class <C-number>;

//  Pointer Designator Types
//
//  These designator types represent pointers. All but the top-level types
//  are directly instantiable and map to themselves for import and export.

// define primary open abstract functional class <C-pointer> (<C-value>)
//   // XXX - Why is this class open? It has two complete, mutually
//   // exclusive subclasses which could be open.
//   constant slot pointer-address :: <raw-pointer>,
//     required-init-keyword: %address:;
// end class <C-pointer>;

// Numeric types

define sealed abstract class <C-int> (<C-number>)
end class;
define sealed abstract class <C-signed-int> (<C-number>)
end class;
define sealed abstract class <C-unsigned-int> (<C-number>)
end class;

define sealed abstract class <C-long> (<C-number>)
end class;
define sealed abstract class <C-signed-long> (<C-number>)
end class;
define sealed abstract class <C-unsigned-long> (<C-number>)
end class;

define sealed abstract class <C-short> (<C-number>)
end class;
define sealed abstract class <C-signed-short> (<C-number>)
end class;
define sealed abstract class <C-unsigned-short> (<C-number>)
end class;

define sealed abstract class <C-char> (<C-number>)
end class;
define sealed abstract class <C-signed-char> (<C-number>)
end class;
define sealed abstract class <C-unsigned-char> (<C-number>)
end class;

// Foreign functions and variables by name
define variable *foreign-table* :: <table> = make(<table>);

// Foreign library names by handle
define variable *foreign-library-table* :: <table> = make(<table>);

// Load a foreign library.
// This is usually a platform dynamicly loadable library or shared library.
define method load-foreign (filename :: <string>) => h :: <foreign-pointer>;
  let h :: <foreign-pointer> = %load-foreign(filename);
  *foreign-library-table*[h] := filename;
  h;
end method;

// Foreign function handle class.

define class <ff> (<object>)
  slot name        :: <symbol>,                 required-init-keyword: name:;
  slot file-handle :: <foreign-pointer>, required-init-keyword: file-handle:;
  slot arg-types   :: <list>,                   init-keyword: arg-types:,
    init-value: #();
  slot return-type :: <class>,                  init-keyword: return-type:,
    init-value: #f;
  slot pointer     :: <foreign-pointer>,        init-value: 0;
end class <ff>;

define method initialize
    (ff :: <ff>,
     #key name: name_ :: <symbol>,
     file-handle: file-handle_,
     arg-types: arg-types_,
     return-type: return-type_,
     pointer: pointer_,
     #all-keys)

  next-method();

  // XXX This is totally bogus!
//  ff.name := %keyword->symbol(ff.name);
//  let s :: <string> = as(<string>, ff.name);
//  ff.name := as(<symbol>, copy-sequence(s, start: 0, end: size(s) - 1));
//  ff.name := as(<symbol>, s);

  // Get the pointer
  ff.pointer := %get-foreign(ff.file-handle, ff.name,
                             ff.arg-types, ff.return-type);

  // Set up a method that will call the foreign function.
  let f = method(#rest args)
//               print("arg-types = "); print(ff.arg-types);
//               print("args = "); print(args);
//               print("map(object-class, args) = ");
//               print(map(object-class, args));
//              if (ff.arg-types = map(object-class, args))
                apply(%call-foreign, ff.pointer, args);
//               else
//                  // Adequate?
//                  error("Foreign function args don't match spec: ",
//                        ff.name);
//                end;
           end;

  // Hack to put the binding in the top level.
//  %add-top-level-binding(pair(ff.name, <method>), f);
//  %add-top-level-binding(pair(as(<symbol>, s), <method>), f);
  %add-top-level-binding(pair(ff.name, <method>), f);

  // Save it in the table
  *foreign-table*[ff.name] := ff;

  ff;
end method;

///////////////////////////////////////////////////////////////////////////
// Foreign variable handle

define class <fv> (<object>)
  slot name        :: <symbol>,          required-init-keyword: name:;
  slot file-handle :: <foreign-pointer>, required-init-keyword: file-handle:;
  slot type	   :: <class>,           required-init-keyword: type:;
  slot pointer     :: <foreign-pointer>, init-value: 0;
end class <ff>;

define method initialize
    (fv :: <fv>,
     #key name: name_ :: <symbol>,
     file-handle: file-handle_,
     type: type_,
     pointer: pointer_,
     #all-keys)

  next-method();

  // Get the pointer
  fv.pointer := %get-foreign-variable(fv.file-handle, fv.name, fv.type);

  // Add a getter method
  let f = method()
              %get-foreign-value(fv.pointer);
          end;

  // Make the getter
  %add-top-level-binding(pair(fv.name, <method>), f);

  // Save it in the table
  *foreign-table*[fv.name] := fv;

  fv;
end method;

///////////////////////////////////////////////////////////////////////////
//define variable md5_fh = load-foreign("/sw/lib/libmd5.0.1.0.dylib");
//define variable md5_ff = make(<ff>,
//                              name: #"MD5",
//                              file-handle: md5_fh,
//                              arg-types: vector(<string>,<integer>,<string>));
//
// unsigned char *MD5(const unsigned char *d, unsigned long n,
//                    unsigned char *md);
//
// void MD5_Init(MD5_CTX *c);
// void MD5_Update(MD5_CTX *c, const void *data, unsigned long len);
// void MD5_Final(unsigned char *md, MD5_CTX *c);
//

/* 

<C-int>                         int             <integer>
<C-raw-int>                     int             <machine-word>
<C-unsafe-int>                  int             <integer>
<C-raw-signed-int>		signed int      <machine-word> 
<C-unsafe-signed int>           signed int      <integer>
<C-signed-int>                  signed int      <integer>
<C-raw-unsigned-int>            unsigned int    <machine-word>
<C-unsafe-unsigned-int>         unsigned int    <integer>
<C-unsigned-int>                unsigned int    <integer>
<C-unsigned-long>               unsigned long   <integer>
<C-signed-long>                 signed long     <integer>
<C-unsafe-unsigned-long>        unsigned long   <integer>
<C-unsafe-signed-long>          signed long     <integer>
<C-raw-unsigned-long>           unsigned long   <machine-word>
<C-raw-signed-long>             signed long     <machine-word>
<C-unsigned-short>              unsigned short  <integer>
<C-signed-short>                signed short    <integer>
<C-unsafe-unsigned-short>	unsigned short	<integer>
<C-unsafe-signed-short>         signed short    <integer>
<C-raw-unsigned-short>          unsigned short  <machine-word>
<C-raw-signed-short>            signed short    <machine-word>
<C-unsigned-char>               unsigned char   <integer>
<C-signed-char>                 signed char     <integer>
<C-unsafe-unsigned-char>        unsigned char   <integer>
<C-unsafe-signed-char>          signed char     <integer>
<C-raw-unsigned-char>           unsigned char   <machine-word>
<C-raw-signed-char>             signed char     <machine-word>
<C-char>                        char            <integer>
<C-unsafe-char>                 char            <integer>
<C-raw-char>                    char            <machine-word>

<C-int>                         int             <integer>
<C-signed-int>                  signed int      <integer>
<C-unsigned-int>                unsigned int    <integer>
<C-unsigned-long>               unsigned long   <integer>
<C-signed-long>                 signed long     <integer>
<C-unsigned-short>              unsigned short  <integer>
<C-signed-short>                signed short    <integer>
<C-unsigned-char>               unsigned char   <integer>
<C-signed-char>                 signed char     <integer>
<C-char>                        char            <integer>

<C-raw-int>                     int             <machine-word>
<C-raw-signed-int>		signed int      <machine-word> 
<C-raw-unsigned-int>            unsigned int    <machine-word>
<C-raw-unsigned-long>           unsigned long   <machine-word>
<C-raw-signed-long>             signed long     <machine-word>
<C-raw-unsigned-short>          unsigned short  <machine-word>
<C-raw-signed-short>            signed short    <machine-word>
<C-raw-unsigned-char>           unsigned char   <machine-word>
<C-raw-signed-char>             signed char     <machine-word>
<C-raw-char>                    char            <machine-word>

<C-unsafe-int>                  int             <integer>
<C-unsafe-signed int>           signed int      <integer>
<C-unsafe-unsigned-int>         unsigned int    <integer>
<C-unsafe-unsigned-long>        unsigned long   <integer>
<C-unsafe-signed-long>          signed long     <integer>
<C-unsafe-unsigned-short>	unsigned short	<integer>
<C-unsafe-signed-short>         signed short    <integer>
<C-unsafe-unsigned-char>        unsigned char   <integer>
<C-unsafe-signed-char>          signed char     <integer>
<C-unsafe-char>                 char            <integer>

*/
