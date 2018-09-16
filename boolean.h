/*
** boolean.h
*/

#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "object.h"

void init_boolean_prims (void);
Object make_true (void);
Object make_false (void);
Object id_p (Object obj1, Object obj2, Object rest);
Object not_id_p (Object obj1, Object obj2, Object rest);
int id (Object obj1, Object obj2);

#endif /* BOOLEAN_H */
