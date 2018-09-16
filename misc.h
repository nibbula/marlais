/*
** misc.h
*/

#ifndef MISC_H
#define MISC_H

#include "object.h"

Object make_eof_object (void);
Object make_unspecified_object (void);
Object make_uninit_slot (void);
Object make_exit (Object sym);
Object make_unwind (Object body);

#endif /* MISC_H */
