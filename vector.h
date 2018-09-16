/*
** vector.h
*/

#ifndef VECTOR_H
#define VECTOR_H

#include "object.h"

void init_vector_prims (void);
Object make_sov (Object el_list);
Object make_vector (int size, Object fill_obj);
Object make_vector_driver (Object args);
Object vector_to_list (Object vec);
Object vector_element (Object vec, Object index, Object default_ob);

#endif /* VECTOR_H */
