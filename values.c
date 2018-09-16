/*
** values.c
*/

#include "values.h"

#include "alloc.h"
#include "error.h"
#include "list.h"
#include "prim.h"

/* primitives */

static struct primitive values_prims[] =
{
  {"values", prim_0_rest, values},
};

/* function definitions */

void
init_values_prims (void)
{
  int num = sizeof (values_prims) / sizeof (struct primitive);
  init_prims (num, values_prims);
}

Object
make_values (Object vals)
{
  Object obj;
  int i;

  if (EMPTYLISTP (vals)) {
    return unspecified_object;
  } else {
    obj = allocate_object (sizeof (struct values));

    VALUESTYPE (obj) = Values;
    VALUESNUM (obj) = list_length (vals);
    VALUESELS (obj) = (Object *)
      checking_malloc (VALUESNUM (obj) * sizeof (Object));

    for (i = 0; i < VALUESNUM (obj); ++i) {
      VALUESELS (obj)[i] = CAR (vals);
      vals = CDR (vals);
    }
    return (obj);
  }
}

Object
construct_values (int num,...)
{
  Object obj;
  int i;
  va_list args;

  obj = allocate_object (sizeof (struct values));
  
  VALUESTYPE (obj) = Values;
  VALUESNUM (obj) = num;
  VALUESELS (obj) = (Object *) checking_malloc (num * sizeof (Object));

  va_start (args, num);
  for (i = 0; i < num; ++i) {
    VALUESELS (obj)[i] = va_arg (args, Object);
  }
  va_end (args);
  return (obj);
}

Object
values (Object rest)
{
  if (EMPTYLISTP (rest) || PAIRP (CDR (rest))) {
    return make_values (rest);
  } else {
    return (CAR (rest));
  }
}

Object
devalue (Object val)
{
  if (VALUESP (val)) {
    if (VALUESNUM (val)) {
      return VALUESELS (val)[0];
    } else {
      return error ("Null values construct used in an invalid context", NULL);
    }
  } else {
    return val;
  }
}
