/*
** sequence.c -- general operations that all sequences use
*/

#include "sequence.h"
#include "globaldefs.h"
#include "error.h"

void make_sequence_driver(Object args, 
	int* size, Object* size_obj, Object* fill_obj,
	const char* type)
{
  *size = 0;
  *size_obj = NULL;
  *fill_obj = false_object;

  while (!EMPTYLISTP (args)) {
    if (FIRST (args) == size_keyword) {
      *size_obj = SECOND (args);
    } else if (FIRST (args) == fill_keyword) {
      *fill_obj = SECOND (args);
    } else {
      char err_msg[80];
      snprintf(err_msg, 80, "make: unsupported keyword for %s class", type);
      error (err_msg, FIRST (args), NULL);
    }
    args = CDR (CDR (args));
  }
  if (*size_obj) {
    if (!INTEGERP (*size_obj)) {
      error ("make: value of size: argument must be an integer",
	     size_obj, NULL);
    }
    *size = INTVAL (*size_obj);
  }
}
