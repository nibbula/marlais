/*
**  deque.h
*/

#ifndef DEQUE_H
#define DEQUE_H

#include "object.h"

/* global objects */
void init_deque_prims (void);
Object make_deque (void);
Object make_deque_entry (Object prev, Object value, Object next);
Object make_deque_driver (Object args);

#endif /* DEQUE_H */
