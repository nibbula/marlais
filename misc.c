/*
** misc.c
*/

#include "misc.h"

#include "alloc.h"
#include "prim.h"

Object
make_eof_object (void)
{
#ifndef SMALL_OBJECTS
    Object obj = allocate_object (sizeof (struct object));
    TYPE (obj) = EndOfFile;
    return (obj);
#else
    return (EOFVAL);
#endif
}

Object
make_unspecified_object (void)
{
    Object obj = allocate_object (sizeof (struct values));

    VALUESTYPE (obj) = Values;
    VALUESNUM (obj) = 0;
    VALUESELS (obj) = NULL;

    return obj;
}

Object
make_uninit_slot (void)
{
#ifndef SMALL_OBJECTS
    Object obj = allocate_object (sizeof (struct object));
    TYPE (obj) = UninitializedSlotValue;
    return (obj);
#else
    return (UNINITVAL);
#endif
}

/* jump buffer must be filled in by
   calling routine. */
Object
make_exit (Object sym)
{
    Object obj = allocate_object (sizeof (struct exitproc));
    EXITTYPE (obj) = Exit;
    EXITSYM (obj) = sym;
    EXITRET (obj) = (jmp_buf *) checking_malloc (sizeof (jmp_buf));
    return (obj);
}

Object
make_unwind (Object body)
{
    Object obj = allocate_object (sizeof (struct unwind));
    UNWINDTYPE (obj) = Unwind;
    UNWINDBODY (obj) = body;
    return (obj);
}

