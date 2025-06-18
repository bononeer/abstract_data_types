#ifndef _STACK_AUX_H
#define _STACK_AUX_H

#include "adt/stack.h"
#include "aux/aux.h"

/* ############################### Operations ############################### */

bool stack_resize(stack_t *stack, size_t new_capacity);

bool err_empty_stack(stack_t *stack);

#endif // _STACK_AUX_H