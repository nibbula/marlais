/*
** boolean.c
*/

#include "boolean.h"

#include "prim.h"

/* primitives */

static Object not (Object obj);

static struct primitive boolean_prims[] =
{
  /*    {"%~==", prim_2_rest, not_id_p}, */
  {"%identical?", prim_2_rest, id_p},
  {"%not", prim_1, not},
};

/* function definitions */

void
init_boolean_prims (void)
{
  int num = sizeof (boolean_prims) / sizeof (struct primitive);
  init_prims (num, boolean_prims);
}

#ifndef SMALL_OBJECTS
Object
make_true (void)
{
  Object obj = allocate_object (sizeof (struct object));

  TYPE (obj) = True;
  return (obj);
}
#else
Object
make_true (void)
{
  return (TRUEVAL);
}
#endif

#ifndef SMALL_OBJECTS
Object
make_false (void)
{
  Object obj = allocate_object (sizeof (struct object));

  TYPE (obj) = False;
  return (obj);
}
#else
Object
make_false (void)
{
  return (FALSEVAL);
}
#endif

Object
id_p (Object obj1, Object obj2, Object rest)
{
  /* succeed quickly in the simple case */
  if ((obj1 == obj2)) {
    if (EMPTYLISTP (rest)) {
      return (true_object);
    } else {
      return id_p (obj2, CAR (rest), CDR (rest));
    }
  } else if (INTEGERP (obj1) && INTEGERP (obj2)) {
    if (INTVAL (obj1) == INTVAL (obj2)) {
      if (EMPTYLISTP (rest)) {
	return (true_object);
      } else {
	return (id_p (obj2, CAR (rest), CDR (rest)));
      }
    } else {
      return (false_object);
    }
  } else if (CHARP (obj1) && CHARP (obj2)) {
    if (CHARVAL (obj1) == CHARVAL (obj2)) {
      if (EMPTYLISTP (rest)) {
	return (true_object);
      } else {
	return (id_p (obj2, CAR (rest), CDR (rest)));
      }
    } else {
      return (false_object);
    }
  } else if (DFLOATP (obj1) && DFLOATP (obj2)) {
    if (DFLOATVAL (obj1) == DFLOATVAL (obj2)) {
      if (EMPTYLISTP (rest)) {
	return (true_object);
      } else {
	return (id_p (obj2, CAR (rest), CDR (rest)));
      }
    } else {
      return (false_object);
    }
  } else {
    return (false_object);
  }
}

int
id (Object obj1, Object obj2)
{
  if (obj1 == obj2) {
    return 1;
  } else if (INTEGERP (obj1) && INTEGERP (obj2)) {
    return (INTVAL (obj1) == INTVAL (obj2));
  } else if (CHARP (obj1) && CHARP (obj2)) {
    return (CHARVAL (obj1) == CHARVAL (obj2));
  } else if (DFLOATP (obj1) && DFLOATP (obj2)) {
    return (DFLOATVAL (obj1) == DFLOATVAL (obj2));
  } else {
    return 0;
  }
}

static Object
not (Object obj)
{
  if (obj == false_object) {
    return (true_object);
  } else {
    return (false_object);
  }
}
