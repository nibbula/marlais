/*
** number.h
*/

#ifndef NUMBER_H
#define NUMBER_H

#include "object.h"

void init_number_prims (void);
Object make_integer (int i);
Object make_ratio (int numerator, int denominator);
Object make_sfloat (float f);
Object make_dfloat (double d);

#endif /* NUMBER_H */
