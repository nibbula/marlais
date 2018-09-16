/*
** sys.h
*/

#ifndef SYS_H
#define SYS_H

#include "object.h"

void init_sys_prims (void);
Object get_ctime (void);
Object get_time (void);
Object get_clock (void);
Object user_system (Object string);
Object user_getenv (Object string);
Object user_random ();
Object user_srandom (Object seed);

#endif /* SYS_H */
