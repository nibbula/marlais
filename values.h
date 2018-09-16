/*
** values.h
*/

#ifndef VALUES_H
#define VALUES_H

#include <stdarg.h>
#include "object.h"

void init_values_prims (void);
Object make_values (Object vals);
Object values (Object rest);
Object construct_values (int num,...);
Object devalue (Object val);

#endif /* VALUES_H */
