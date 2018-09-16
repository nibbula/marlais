/*
** sequence.h -- general operations that all sequences use
*/

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "object.h"

void make_sequence_driver(Object args, 
			  int* sz, Object* sz_obj, Object* fill, 
			  const char* type);

#endif /* SEQUENCE_H */
