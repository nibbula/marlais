/*
** macintosh.h -- Dylan primitives for Macintosh.
**
** by Patrick C. Beard.
*/

#pragma once

#include "object.h"

void init_mac_prims (void);
Object get_file (void);
Object put_file (Object defaultNameObj, Object promptObj);
char *FSSpecToPath (struct FSSpec *file, char path[256]);
