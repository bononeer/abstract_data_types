#ifndef _STACK_H
#define _STACK_H

#include <stdbool.h>
#include <stddef.h>

/******************** Stack structure definition ********************/ 

typedef struct stack_t* Stack;

/******************** Stack functions declarations ********************/

/* Returns an instance of an empty stack. 

PRE:
- elem_destroy is a pointer to a function that destroys the elements stored in the stack.
If NULL is given, it will destroy the elements with the `free` function from stdlib.

POST:
- if there is not enough memory for the stack, the function will return NULL. */
Stack stack_create(void (*elem_destroy)(void* elem));

/* Frees the memory where the stack is allocated */
void stack_destroy(Stack stack);

/* Add `elem`'s value into the top of the stack. 

POST:
- Returns true if the item was successfully added to the stack, and false if there was an 
issue with the operation. */
bool stack_push(Stack stack, void* elem);

/* Delete and return the value of the element at the top of the stack. If the stack does not
have any element to pop, it must abort the flow. 

POST:
- If the queue is empty, a NULL pointer will be returned.
- The returned pointer should be freed when not needed anymore.
- A void pointer will be returned, the user should cast it to the right type. */
void* stack_pop(Stack stack);

/* Return the value of the element at the top of the stack. If the stack does not have any 
element to pop, it must abort the flow. 

POST:
- If the queue is empty, a NULL pointer will be returned.
- A void pointer will be returned, the user should cast it to the right type.*/
void* stack_top(const Stack stack);

/* Returns true if the stack is empty; otherwise, it returns false. */
bool stack_is_empty(const Stack stack);

#endif // _STACK_H