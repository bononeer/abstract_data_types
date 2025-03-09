#ifndef _STACK_H
#define _STACK_H

#include <stdbool.h>
#include <stddef.h>
#include "additional_types.h"

/******************** Stack structure declaration ********************/

// A data structure that follows the LIFO principle.
typedef struct stack_t *Stack;

/******************** Stack operations declarations ********************/

/* Returns an instance of an empty Stack. 

PRE:
- `elem_destroy` is a pointer to a `destroy_func_t` that defines how to free the memory
of the elements stored in the Stack. If NULL is given, it will not free the memory of 
the elements. 

POST:
- if there is not enough memory for the Stack, the function will return NULL. */
Stack stack_create(destroy_func_t elem_destroy);

/* Frees the memory where the Stack is allocated. */
void stack_destroy(Stack stack);

/* Returns true if the Stack is empty; otherwise, it returns false. */
bool stack_is_empty(const Stack stack);

/* Adds an element to the top of the Stack. 

POST:
- Returns true if the item was successfully added to the Stack, and false if there was an 
issue with the operation. */
bool stack_push(Stack stack, void *elem);

/* Return the element at the top of the Stack. 

POST:
- If the Stack is empty, a NULL pointer will be returned. */
void *stack_top(const Stack stack);

/* Remove the element at the top of the Stack and return it. 

POST:
- If the Stack is empty, a NULL pointer will be returned.
- If the memory was allocated previously, the memory of the returned element should be 
freed when not needed anymore. */
void *stack_pop(Stack stack);

#endif // _STACK_H