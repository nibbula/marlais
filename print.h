/*
** print.h
*/

#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>
#include "object.h"

/* global objects */
extern Object instance_symbol;

void pr (Object obj);
void debug (CONST char * msg, ...);
void init_print_prims (void);
void print_object (Object stream, Object obj, int escaped);
Object print_obj (Object stream, Object obj);
void print_err (Object obj);
Object find_getter_name (Object slot);
Object format (Object stream, Object str, Object rest);

#endif /* PRINT_H */
