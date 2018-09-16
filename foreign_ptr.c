/*
** foreign_ptr.c
*/

#include "foreign_ptr.h"

#ifndef SMALL_OBJECTS
Object
make_foreign_ptr (void *ptr)
{
    Object obj;

    obj = allocate_object (sizeof (struct object));

    TYPE (obj) = ForeignPtr;
    FOREIGNPTR (obj) = ptr;
    return (obj);
}
#else /* !SMALL_OBJECTS */
Object
make_foreign_ptr (void *ptr)
{
    Object obj;

    obj = allocate_object (sizeof (struct foreign_ptr));

    FOREIGNTYPE (obj) = ForeignPtr;
    FOREIGNPTR (obj) = ptr;
    return (obj);
}
#endif /* !SMALL_OBJECTS */
