/*
** sys.c
*/

#include <time.h>
#include <stdlib.h>

#include "sys.h"

#include "bytestring.h"
#include "boolean.h"
#include "error.h"
#include "number.h"
#include "prim.h"
#include "values.h"

#define POSITIVE_SMALL_INT_MASK	01777777777

static struct primitive sys_prims[] =
{
    {"ctime", prim_0, get_ctime},
    {"time", prim_0, get_time},
    {"clock", prim_0, get_clock},
    {"system", prim_1, user_system},
    {"getenv", prim_1, user_getenv},
    {"%random", prim_0, user_random},
    {"%srandom", prim_1, user_srandom},
};

void
init_sys_prims (void)
{
    int num;

    num = sizeof (sys_prims) / sizeof (struct primitive);

    init_prims (num, sys_prims);
}

Object
get_ctime ()
{
    time_t time_loc;

    (void) time (&time_loc);
    return make_byte_string (ctime (&time_loc));
}

Object
get_time ()
{
    time_t time_loc;

    (void) time (&time_loc);

#ifndef SMALL_OBJECTS
    time_loc = abs ((int) time_loc);
#else
    time_loc &= POSITIVE_SMALL_INT_MASK;
#endif

    return make_integer (time_loc);
}

Object
get_clock ()
{
    clock_t clock_val;

    clock_val = clock ();

#ifndef SMALL_OBJECTS
    clock_val = abs ((int) clock_val);
#else
    clock_val = abs ((int) (clock_val << 2)) >> 2;
#endif

    return make_integer (clock_val);
}

Object
user_system (Object string)
{
    if (!BYTESTRTYPE (string)) {
	return error ("system: argument must be a string", string, NULL);
    } else {
	return make_integer (system (BYTESTRVAL (string)));

    }
}

// @@@ Perhaps this should be called %getenv and have a coreesponding
// init.dylan method.
Object
user_getenv (Object string)
{
    if (!BYTESTRTYPE (string)) {
	return error ("getenv: argument must be a string", string, NULL);
    } else {
	char * str = getenv(BYTESTRVAL(string));
	if (str)
	    return make_byte_string (str);
	else
	    return make_false();
    }
}

// This could perhaps use some more argument types
Object
user_srandom (Object seed)
{
    if (!INTEGERP(seed)) {
	return error ("srandom: argument must be an integer", seed, NULL);
    } else {
	srandom((unsigned long)INTVAL(seed));
    }
    return unspecified_object;
}

Object
user_random ()
{
    return make_integer(random());
}

/*
** Local variables:
** c-basic-offset: 4
** End:
*/
