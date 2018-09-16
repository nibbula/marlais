/*
** slot.h
*/

#ifndef SLOT_H
#define SLOT_H

#include "object.h"

void init_slot_prims (void);
Object slot_name (Object slot);
Object slot_getter (Object slot);
Object slot_setter (Object slot);
Object slot_type (Object slot);
Object slot_init_value (Object slot);
Object slot_init_function (Object slot);
Object slot_init_keyword (Object slot);
Object slot_required_init_keyword (Object slot);
Object slot_allocation (Object slot);
Object slot_value (Object instance, Object name);
Object set_slot_value (Object instance, Object name, Object val);
Object make_slot_descriptor (unsigned char properties, Object getter,
			     Object setter, Object type, Object init,
			     Object init_keyword, Object allocation,
			     Object dynamism);

#endif /* SLOT_H */
