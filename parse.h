/*
** parse.h
*/

#ifndef PARSE_H
#define PARSE_H

#include "object.h"

extern int load_file_context;

Object parse_object (FILE * fp, int debug);
void reset_parser (FILE * fp);

#endif /* PARSE_H */
