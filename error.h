/*
**  error.h
*/

#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>
#include "object.h"

void init_error_prims (void);
void fatal (CONST char * msg);
Object error (CONST char * msg,...);
Object warning (CONST char * msg,...);

extern int num_debug_contexts;
extern int NoDebug;

#endif /* ERROR_H */
