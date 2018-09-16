/*
** parse.c
*/

#include <stdio.h>

#include "parse.h"

#include "error.h"
#include "print.h"
#include "yystype.h"

Object *parse_value_ptr;
extern FILE *yyin;
extern Object standard_error_stream;

void yy_restart (FILE * new_file);

extern int yydebug;
extern int echo_prefix;
extern int yychar;
extern int yylineno;
extern int yybyte;
extern int load_file_context;

extern Object eof_object;

Object
parse_object (FILE * fp, int debug)
{
  Object parse_value;

  if (!yyin) {
    yyin = fp;
  } else if (yyin && fp != yyin) {
    reset_parser (fp);
  }
  yydebug = debug;
  parse_value_ptr = &parse_value;
  if (yyparse () == 0) {
    return parse_value;
  } else {
    warning ("Parser failed in inexplicable way", parse_value, NULL);
    return eof_object;
  }
}

void
reset_parser (FILE * fp)
{
  fflush (stderr);
  yy_restart (fp);
  yylineno = 1;
  yybyte = 0;
}
