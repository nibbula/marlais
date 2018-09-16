/*
** vector.c
*/

#include "vector.h"

#include "globaldefs.h"
#include "alloc.h"
#include "error.h"
#include "list.h"
#include "number.h"
#include "prim.h"
#include "symbol.h"
#include "sequence.h"

/* primitives */

static Object vector_element_setter (Object vec, Object index, Object val);
static Object vector_size (Object vec);

static struct primitive vector_prims[] =
{
    {"%vector-element", prim_3, vector_element},
    {"%vector-element-setter", prim_3, vector_element_setter},
    {"%vector-size", prim_1, vector_size},
    {"%vector", prim_1, make_sov},
};

/* function definitions */
void
init_vector_prims (void)
{
  int num = sizeof (vector_prims) / sizeof (struct primitive);
  init_prims (num, vector_prims);
}

Object
make_sov (Object el_list)
{
  Object obj, els;
  int size, i;

  obj = allocate_object (sizeof (struct simple_object_vector));

  SOVTYPE (obj) = SimpleObjectVector;
  size = 0;
  els = el_list;
  while (PAIRP (els)) {
    size++;
    els = CDR (els);
  }
  SOVSIZE (obj) = size;
  SOVELS (obj) = (Object *) checking_malloc (size * sizeof (Object));

  els = el_list;
  i = 0;
  while (PAIRP (els)) {
    SOVELS (obj)[i++] = CAR (els);
    els = CDR (els);
  }
  return (obj);
}

Object
make_vector (int size, Object fill_obj)
{
  Object res;
  int i;

  /* actually fabricate the vector */
  res = allocate_object (sizeof (struct simple_object_vector));

  SOVTYPE (res) = SimpleObjectVector;
  SOVSIZE (res) = size;
  SOVELS (res) = (Object *) checking_malloc (size * sizeof (Object));

  for (i = 0; i < size; ++i) {
    SOVELS (res)[i] = fill_obj;
  }
  return (res);
}

/* Called with args to make */
Object
make_vector_driver (Object args)
{
  int size;
  Object size_obj, fill_obj;

  make_sequence_driver(args, &size, &size_obj, &fill_obj, "<vector>");
  return make_vector (size, fill_obj);
}

Object
vector_to_list (Object vec)
{
  int i;
  Object first, cur, acons;

  cur = make_empty_list ();
  for (i = 0; i < SOVSIZE (vec); ++i) {
    acons = cons (SOVELS (vec)[i], make_empty_list ());
    if (!EMPTYLISTP (cur)) {
      CDR (cur) = acons;
    } else {
      first = acons;
    }
    cur = acons;
  }
  return (first);
}

/* primitives */

Object
vector_element (Object vec, Object index, Object default_ob)
{
  int i, size;
    
  i = INTVAL (index);
  size = SOVSIZE (vec);
  if ((i < 0) || (i >= size)) {
    if (default_ob == default_object) {
      error ("element: index out of range", vec, index, NULL);
    } else {
      return default_ob;
    }
  }
  return (SOVELS (vec)[i]);
}

static Object
vector_element_setter (Object vec, Object index, Object val)
{
  int i, size;

  i = INTVAL (index);
  size = SOVSIZE (vec);
  if ((i < 0) || (i >= size)) {
    error ("element-setter: index out of range", vec, index, NULL);
  }
  return (SOVELS (vec)[i] = val);
}

static Object
vector_size (Object vec)
{
  return (make_integer (SOVSIZE (vec)));
}
