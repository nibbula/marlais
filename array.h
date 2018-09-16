/*
** array.h
*/

#ifndef ARRAY_H
#define ARRAY_H

#include "object.h"

/* globals */
extern Object dim_keyword, fill_keyword, default_object;

void init_array_prims (void);
Object make_array (Object dims, Object fill);
Object make_array_driver (Object args);

#endif /* ARRAY_H */
