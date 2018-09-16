/*
** symbol.h
*/

#ifndef SYMBOL_H
#define SYMBOL_H

#include "object.h"

/* Is this really necessary? e.g. gcc converts % to & when possible anyway */
#ifdef BITWISE_MOD_OPTIMIZATION
# define TAB_MOD(h, s) ((h) & (s - 1)) /* Works only if s is a power of 2 */
#else
# define TAB_MOD(h, s) ((h) % (s))
#endif

struct symtab {
    Object sym;
    struct symtab *next;
};

Object make_symbol (char *name);
Object make_keyword (char *name);
Object make_setter_symbol (Object sym);
void init_symbol_prims (void);

#endif /* SYMBOL_H */
