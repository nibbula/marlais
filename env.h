/*
**  env.h
*/

#ifndef ENV_H
#define ENV_H

#include "object.h"

struct binding {
    Object sym, *val, type;
    int props;
    struct binding *next;	/* in top_level_env hash chain */
};

#define CONSTANT_BINDING 0x01
#define IMPORTED_BINDING 0x02
#define EXPORTED_BINDING 0x04

#define IS_IMPORTED_BINDING(binding) (binding->props & IMPORTED_BINDING)
#define IS_CONSTANT_BINDING(binding) (binding->props & CONSTANT_BINDING)
#define IS_EXPORTED_BINDING(binding) (binding->props & EXPORTED_BINDING)


struct module_binding {
    Object sym;
    struct frame *namespace;
    Object exported_bindings;
};

struct frame {
    int size;
    Object owner;
    struct binding **bindings;
    struct frame *next;
    struct binding **top_level_env;
};

extern struct frame *the_env;
extern Object default_module;
extern Object all_symbol;

void init_env_prims (void);

void add_top_lvl_binding1(Object sym, Object val, int constant, int exported);
void add_top_level_binding(Object sym, Object val, int constant);
void push_scope (Object owner);
void pop_scope (void);
Object print_env (struct frame *env);
Object show_bindings (Object args);

/* Warning!!! - you can't mix calls to add_bindings() and
   add_binding() within the same frame.  Things will get
   hopelessly screwed up.
 */
void add_bindings (Object syms, Object vals, int constant,
		   struct frame *to_frame);
void add_binding (Object sym, Object val, int constant,
		  struct frame *to_frame);
int change_binding (Object sym, Object val);

Object symbol_value (Object sym);
void modify_value (Object sym, Object new_val);
struct frame *current_env (void);
int unwind_to_exit (Object exit_sym);
struct binding *symbol_binding_top_level (Object sym);
struct frame *module_namespace ();
struct module_binding *new_module (Object module_name);
struct module_binding *set_module (struct module_binding *module);
struct module_binding *find_module (Object module_name);
Object use_module (Object module_name,
		   Object imports,
		   Object exclusions,
		   Object prefix,
		   Object renames,
		   Object exports);
Object user_set_module (Object args);
struct module_binding *current_module (void);
struct module_binding *module_binding (Object module_name);
Object make_environment (struct frame *env);
void fill_table_from_property_set (Object the_table, Object the_set);

#endif /* ENV_H */
