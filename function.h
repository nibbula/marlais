/*
** function.h
*/

#ifndef FUNCTION_H
#define FUNCTION_H

#include "object.h"
/* global objects */
extern Object allkeys_symbol, all_symbol;

void init_function_prims (void);
Object add_method (Object generic, Object method);
Object make_generic_function (Object name, Object params, Object methods);
Object make_method (Object name, Object params, Object body, struct frame *env, int do_generic);
Object make_next_method (Object generic, Object rest_methods, Object args);
Object make_generic_function_driver (Object args);
Object sorted_applicable_methods (Object fun, Object sample_args);
Object function_specializers (Object meth);
Object applicable_method_p (Object fun, Object sample_args, int strict_check);

#ifdef USE_METHOD_CACHING
Object recalc_next_methods (Object fun, Object meth, Object sample_args);
Object sorted_possible_method_handles (Object fun, Object sample_args);

#endif

#endif /* FUNCTION_H */
