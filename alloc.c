/*
** alloc.c
*/

#include <string.h>

#include "alloc.h"

#ifdef DEBUG
# include "file.h"		/* for load_filename */
extern int yylineno;
extern int yybyte;

# define GC_DEBUG
#endif

#include "env.h"
#include "symbol.h"

#if defined(NO_GC) || 0
# define GC_INIT()
# define GC_MALLOC(x)		malloc(x)
# define GC_MALLOC_ATOMIC(x)	malloc(x)
# define GC_REALLOC(p,s)	realloc(p,s)
#else
# include "gc.h"
//# define GC_INIT()		GC_init()
extern void GC_init (void);
#endif
#include "error.h"

/* function definitions */

void
initialize_gc (void)
{
    GC_INIT ();
    return;
}

Object
allocate_object (size_t size)
{
    Object obj;

#ifndef SMALL_OBJECTS
    obj = (Object) GC_MALLOC (sizeof (struct object));

#else
    obj = (Object) GC_MALLOC (size);
#endif
    if (!obj) {
	fatal ("internal error: memory allocation failure.");
    }
    OSET_DEBUG(obj, load_filename, yylineno, yybyte);
    return (obj);
}

struct frame *
allocate_frame (void)
{
    struct frame *frame;

    frame = (struct frame *) GC_MALLOC (sizeof (struct frame));

    if (!frame) {
	fatal ("internal error: memory allocation failure.");
    }
    return (frame);
}

struct binding *
allocate_binding (void)
{
    struct binding *binding;

    binding = (struct binding *) GC_MALLOC (sizeof (struct binding));

    if (!binding) {
	fatal ("internal error: memory allocation failure.");
    }
    return (binding);
}

struct module_binding *
allocate_module_binding (void)
{
    struct module_binding *module_binding;

    module_binding =
	(struct module_binding *) GC_MALLOC (sizeof (struct module_binding));

    if (!module_binding) {
	fatal ("internal error: memory allocation failure.");
    }
    return (module_binding);
}

struct symtab *
allocate_symtab (void)
{
    struct symtab *entry;

    entry = (struct symtab *) GC_MALLOC (sizeof (struct symtab));

    if (!entry) {
	fatal ("internal error: memory allocation failure.");
    }
    return (entry);
}

void *
checking_malloc (size_t size)
{
    void *ptr;

    ptr = (void *) GC_MALLOC (size);
    if (!ptr) {
	fatal ("internal error: memory allocation failure");
    }
    return (ptr);
}

void *
checking_realloc (void *ptr, size_t total_size)
{
    ptr = (void *) GC_REALLOC (ptr, total_size);
    if (!ptr) {
	fatal ("internal error: memory allocation failure");
    }
    return (ptr);
}

char *
checking_strdup (char *str)
{
    int size;
    char *copied_str;

    size = strlen (str) + 1;
    copied_str = (char *) GC_MALLOC_ATOMIC (size);
    if (!copied_str) {
	fatal ("internal error: memory allocation failure");
    }
    strcpy (copied_str, str);
    return (copied_str);
}

char *
allocate_string (size_t size)
{
    char *str;

    str = (char *) GC_MALLOC_ATOMIC (size);
    if (!str) {
	fatal ("internal error: memory allocation failure.");
    }
    return (str);
}
