/*
** prim.h
*/

#ifndef PRIM_H
#define PRIM_H

#include "object.h"

void init_prims (int num, struct primitive prims[]);
Object make_primitive (char *name, enum primtype type, Object (*fun) ());
Object apply_prim (Object prim, Object args);

#endif /* PRIM_H */
