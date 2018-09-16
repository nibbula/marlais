/*
** table.c
*/

#include <string.h>

#include "table.h"

#include "alloc.h"
#include "apply.h"
#include "globaldefs.h"
#include "error.h"
#include "list.h"
#include "number.h"
#include "prim.h"
#include "symbol.h"

extern Object dylan_symbol;

/* local function prototypes */

static Object *table_element_handle (Object table,
				     Object key,
				     Object *default_val);
static Object table_initial_state (Object table);
static Object table_next_state (Object table, Object state);
static Object table_current_element (Object table, Object state);
static Object table_current_key (Object table, Object state);
static Object table_current_element_setter (Object table, Object state, Object value);
static Object equal_hash (Object key);
static Object hash_pair (Object pair);
static Object hash_deque (Object deq);
static Object hash_string (Object string);
static Object hash_vector (Object vector);

static Object table_default = NULL;

/* primitives */

static struct primitive table_prims[] =
{
  {"%table-element", prim_3, table_element},
  {"%table-element-setter", prim_3, table_element_setter},
  {"%table-initial-state", prim_1, table_initial_state},
  {"%table-next-state", prim_2, table_next_state},
  {"%table-current-element", prim_2, table_current_element},
  {"%table-current-key", prim_2, table_current_key},
  {"%table-current-element-setter", prim_3, table_current_element_setter},
  {"%=hash", prim_1, equal_hash},
};

void
init_table_prims (void)
{
  int num = sizeof (table_prims) / sizeof (struct primitive);
  init_prims (num, table_prims);
  table_default = cons (false_object, false_object);
}

Object
make_table (int size)
{
  Object obj = allocate_object (sizeof (struct table));
  
  TABLETYPE (obj) = ObjectTable;
  TABLESIZE (obj) = size;
  TABLETABLE (obj) = (Object *) checking_malloc (sizeof (Object) * size);
  memset (TABLETABLE (obj), 0, sizeof (Object) * size);

  return (obj);
}

Object
make_table_entry (int row, Object key, Object value, Object next)
{
  Object obj = allocate_object (sizeof (struct table_entry));

  TETYPE (obj) = TableEntry;
  TEROW (obj) = row;
  TEKEY (obj) = key;
  TEVALUE (obj) = value;
  TENEXT (obj) = next;
  return (obj);
}

Object
make_table_driver (Object rest)
{
  Object size;

  if (EMPTYLISTP (rest)) {
    return (make_table (DEFAULT_TABLE_SIZE));
  } else if (CAR (rest) == size_keyword) {
    rest = CDR (rest);
    if (EMPTYLISTP (rest)) {
      error ("make: no argument given to size keyword", NULL);
    }
    size = CAR (rest);
    if (!INTEGERP (size)) {
      error ("make: argument to size keyword must be an integer", size, NULL);
    }
    return (make_table (INTVAL (size)));
  } else {
    return error ("make: bad keywords or arguments", rest, NULL);
  }
}

/* local functions */

static int
vectors_equal (Object vec1, Object vec2)
{
  int i;

  if (SOVSIZE (vec1) != SOVSIZE (vec2))
    return (0);
  for (i = 0; i < SOVSIZE (vec1); i++) {
    if (SOVELS (vec1)[i] != SOVELS (vec2)[i])
      return (0);
  }
  return (1);
}

Object *
table_element_by_vector (Object table, Object key)
{
  Object hval, entry;
  int h;

  hval = equal_hash (key);
  h = abs (INTVAL (hval)) % TABLESIZE (table);
  entry = TABLETABLE (table)[h];

  while (entry) {
    if (vectors_equal (key, TEKEY (entry))) {
      return &(TEVALUE (entry));
    }
    entry = TENEXT (entry);
  }
  return (NULL);
}

Object
table_element_setter_by_vector (Object table, Object key, Object val)
{
  Object hval, entry;
  Object *element_handle;
  int h;

  element_handle = table_element_by_vector (table, key);
  if (element_handle) {
    *element_handle = val;
  } else {
    hval = equal_hash (key);
    h = abs (INTVAL (hval)) % TABLESIZE (table);
    
    entry = make_table_entry (h, key, val, TABLETABLE (table)[h]);
    TABLETABLE (table)[h] = entry;
  }
  return (unspecified_object);
}

Object
table_element (Object table, Object key, Object default_val)
{
  return *table_element_handle (table, key, &default_val);
}

static Object *
table_element_handle (Object table, Object key, Object *default_val)
{
  Object hval, equal_fun, entry;
  int h;
  struct frame *old_env;

  hval = equal_hash (key);
  h = abs (INTVAL (hval)) % TABLESIZE (table);
  entry = TABLETABLE (table)[h];
  
  old_env = the_env;
  the_env = module_binding (dylan_symbol)->namespace;
  equal_fun = symbol_value (equal_symbol);

  if (!equal_fun)
    // There's no = defined in the dylan module. We are probably still parsing
    // the init file. Just use the %identical? primitive.
    equal_fun = symbol_value (make_symbol("%identical?"));

  the_env = old_env;

  while (entry) {
    if (apply (equal_fun,
	       cons (TEKEY (entry), cons (key, make_empty_list ())))
	!= false_object) {
      return &(TEVALUE (entry));
    }
    entry = TENEXT (entry);
  }
  if (*default_val != default_object) {
    return default_val;
  } else {
#if 0 /* @@@ */
    return error ("element: no element matching key", key, NULL);
#else
    error ("element: no element matching key", key, NULL);
    return NULL;
#endif
  }
}


Object
table_element_setter (Object table, Object key, Object val)
{
  Object hval, entry;
  Object *element_handle;
  int h;

  if ((element_handle = table_element_handle (table, key, &table_default))
      != &table_default) {
    *element_handle = val;
  } else {
    hval = equal_hash (key);
    h = abs (INTVAL (hval)) % TABLESIZE (table);
    entry = make_table_entry (h, key, val, TABLETABLE (table)[h]);
    TABLETABLE (table)[h] = entry;
  }
  return (unspecified_object);
}

/* iteration protocol */

static Object
table_initial_state (Object table)
{
  int i;

  for (i = 0; i < TABLESIZE (table); ++i) {
    if (TABLETABLE (table)[i]) {
      return (TABLETABLE (table)[i]);
    }
  }
  return (false_object);
}

static Object
table_next_state (Object table, Object state)
{
  int i;

  if (TENEXT (state)) {
    return (TENEXT (state));
  }
  for (i = (TEROW (state) + 1); i < TABLESIZE (table); ++i) {
    if (TABLETABLE (table)[i]) {
      return (TABLETABLE (table)[i]);
    }
  }
  return (false_object);
}

static Object
table_current_element (Object table, Object state)
{
  return (TEVALUE (state));
}

static Object
table_current_key (Object table, Object state)
{
  return (TEKEY (state));
}

static Object
table_current_element_setter (Object table, Object state, Object value)
{
  TEVALUE (state) = value;
  return (unspecified_object);
}

static Object
equal_hash (Object key)
{
  Object hashfun;
  
  if (INSTANCEP (key)) {
    hashfun = symbol_value (equal_hash_symbol);
    /*
     * Need to be able to hash arbitrary instances here!
     */
    if (!hashfun) {
      error ("no =hash method defined for key class", key, NULL);
    }
    return (apply (hashfun, cons (key, make_empty_list ())));
  } else {
    if (INTEGERP (key)) {
      return (key);
    } else if (CHARP (key)) {
      return (make_integer (CHARVAL (key)));
    } else if (TRUEP (key)) {
      return (make_integer (1));
    } else if (FALSEP (key)) {
      return (make_integer (0));
    } else if (EMPTYLISTP (key)) {
      return (make_integer (2));
    } else if (PAIRP (key)) {
      return (hash_pair (key));
    } else if (DEQUEP (key)) {
      return (hash_deque (key));
    } else if (BYTESTRP (key)) {
      return (hash_string (key));
    } else if (SOVP (key)) {
      return (hash_vector (key));
    } else if (SYMBOLP (key) || KEYWORDP (key)) {
      return (make_integer ((int) key));
    } else {
      /* error ("=hash: don't know how to hash object", key, NULL);  */
      return (make_integer (((int) key)));
    }
  }
}

static Object
hash_pair (Object pair)
{
  int h = INTVAL (equal_hash (CAR (pair))) + INTVAL (equal_hash (CDR (pair)));
  return (make_integer (h));
}

static Object
hash_deque (Object deq)
{
  int h = 0;
  Object entry;

  entry = DEQUEFIRST (deq);
  while (!EMPTYLISTP (entry)) {
    h += INTVAL (equal_hash (DEVALUE (entry)));
    entry = DENEXT (entry);
  }
  return (make_integer (h));
}

static Object
hash_string (Object string)
{
  int i, h;

  h = 0;
  for (i = 0; i < BYTESTRSIZE (string); ++i) {
    h += BYTESTRVAL (string)[i];
  }
  return (make_integer (h));
}

static Object
hash_vector (Object vector)
{
  int i, h = 0;
  for (i = 0; i < SOVSIZE (vector); ++i) {
    h += INTVAL (equal_hash (SOVELS (vector)[i]));
  }
  return (make_integer (h));
}
