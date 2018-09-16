/*
** alloc.h
*/

#ifndef ALLOC_H
#define ALLOC_H

#include "object.h"
#include "env.h"

void initialize_gc (void);
Object allocate_object (size_t size);
struct frame *allocate_frame (void);
struct binding *allocate_binding (void);
struct module_binding *allocate_module_binding (void);
struct symtab *allocate_symtab (void);
void *checking_malloc (size_t size);
void *checking_realloc (void *ptr, size_t size);
char *checking_strdup (char *str);
char *allocate_string (size_t size);

#endif /* ALLOC_H */
