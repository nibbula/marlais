/*
** table.h
*/

#ifndef TABLE_H
#define TABLE_H

#include "object.h"

#define DEFAULT_TABLE_SIZE 101

extern Object equal_symbol;

void init_table_prims (void);
Object make_table (int size);
Object make_table_entry (int row, Object key, Object value, Object next);
Object make_table_driver (Object rest);
Object table_element_setter (Object table, Object key, Object val);
Object table_element (Object table, Object key, Object default_val);
Object *table_element_by_vector (Object table, Object key);
Object table_element_setter_by_vector (Object table, Object key, Object val);

#endif /* TABLE_H */
