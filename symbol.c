/*
** symbol.c
*/

#include <string.h>
#include <ctype.h>

#include "symbol.h"

#include "alloc.h"
#include "bytestring.h"

/* local function prototypes 
 */
static Object intern_symbol (char *name);

#ifdef NO_STRCASECMP
int strcasecmp (unsigned char *s1, unsigned char *s2);

#endif

/* local data 
 */

/* If SYMTAB_SIZE is not a power of 2, see change required below. */
#define SYMTAB_SIZE 1024

struct symtab *symbol_table[SYMTAB_SIZE];
unsigned char chartable[1 << sizeof (char) * 8];

/* function definitions 
 */
void
init_symbol_prims (void)
{
  int c;

  for (c = 0; c < (1 << sizeof (char) * 8); c++) {
    chartable[c] = tolower (c);
  }
}

Object
make_symbol (char *name)
{
  Object obj;

  obj = intern_symbol (name);
  return (obj);
}

Object
make_keyword (char *name)
{
  Object obj;

  obj = intern_symbol (name);
  SYMBOLTYPE (obj) = Keyword;
  return (obj);
}


Object
make_setter_symbol (Object sym)
{
  char *name;

  name = allocate_string (sizeof (char) * (1 + strlen (SYMBOLNAME (sym)) +
					   strlen ("-setter")));

  strcpy (name, SYMBOLNAME (sym));
  strcat (name, "-setter");
  return (make_symbol (name));
}

#ifdef NO_STRCASECMP
int
strcasecmp (const char * s1, const char * s2)
{
  unsigned char us1 = s1;
  unsigned char us2 = s2;
  while ((chartable[*us1] == chartable[*us2++])) {
    if (!chartable[*us1++])
      return 0;
  }
  return (chartable[*us1] - chartable[*--us2]);
}
#endif

static Object
intern_symbol (char * name)
{
  int i;
  unsigned h;
  struct symtab *entry;
  Object sym;

  h = i = 0;
  while (name[i]) {
    h += tolower (name[i++]);
  }
  h = TAB_MOD(h, SYMTAB_SIZE);

  entry = symbol_table[h];
  while (entry) {
    if (strcasecmp (name, SYMBOLNAME (entry->sym)) == 0) {
      return (entry->sym);
    }
    entry = entry->next;
  }

    /* not found, create new entry for it. */
  sym = allocate_object (sizeof (struct symbol));

  SYMBOLTYPE (sym) = Symbol;
  SYMBOLNAME (sym) = checking_strdup (name);
  entry = (struct symtab *) allocate_symtab ();
  entry->sym = sym;
  entry->next = symbol_table[h];
  symbol_table[h] = entry;
  return (sym);
}
