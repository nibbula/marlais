/*
** eval.h
*/

#ifndef EVAL_H
#define EVAL_H

#include "object.h"
#include "env.h"

Object eval (Object obj);
Object print_stack (void);

struct eval_stack {
    struct eval_stack *next;
    Object context;
    struct frame *frame;
};

extern struct eval_stack *eval_stack;

void pop_eval_stack (void);
void push_eval_stack (Object obj);

/* <pcb> to support tail recursion. */
Object tail_eval (Object obj);

#endif /* EVAL_H */
