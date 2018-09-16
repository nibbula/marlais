/*
**  classprec-ll.h
*/

#ifndef CLASSPREC2_LL_H_
#define CLASSPREC2_LL_H_

#include "object.h"
#include "class.h"

typedef
struct prec_graph {
    Object *class_vec;
    int *succ_vec;
    int succ_size;
    int num_classes;
} prec_graph;

typedef
struct l_list {
    int size;
    int *vec;
} l_list;

Object compute_class_precedence_list (Object class);

#endif /* CLASSPREC2_LL_H_ */
