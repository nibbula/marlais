/*
** list.h
*/

#ifndef LIST_H
#define LIST_H

#include <stdarg.h>
#include "object.h"

#ifndef SMALL_OBJECTS
Object make_empty_list (void);

#else
#define make_empty_list() EMPTYLISTVAL
#endif

void init_list_prims (void);
void initialize_empty_list (void);
Object make_pair_driver (Object args);
Object make_list_driver (Object args);
Object cons (Object car, Object cdr);
Object second (Object lst);
Object third (Object lst);
Object map (Object (*fun) (Object), Object lst);
Object map2 (Object (*fun) (Object, Object), Object l1, Object l2);
Object list_map1 (Object fun, Object lst);
Object list_map2 (Object fun, Object l1, Object l2);
Object append (Object l1, Object l2);
int member (Object obj, Object lst);
Object member_p (Object obj, Object lst, Object test);
Object listem (Object car,...);
Object list_reduce (Object fun, Object init, Object lst);
Object list_reduce1 (Object fun, Object lst);
Object list_length_int (Object lst);
int list_length (Object lst);
int list_equal (Object l1, Object l2);
Object list_reverse (Object lst);
Object list_reverse_bang (Object lst);
Object copy_list (Object lst);

Object add_new_at_end (Object *lst, Object elt);
Object list_sort (Object lst, Object test);
Object list_sort_bang (Object lst, Object test);

#endif /* LIST_H */
