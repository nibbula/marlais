/*
** bytestring.h
*/

#ifndef BYTESTRING_H
#define BYTESTRING_H

#include "object.h"

/* global objects */
void init_string_prims (void);
Object make_byte_string (char *str);
Object make_string_driver (Object args);

#endif /* BYTESTRING_H */
