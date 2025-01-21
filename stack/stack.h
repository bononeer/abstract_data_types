#ifndef _STACK_H
#define _STACK_H

/******************** Stack structure definition ********************/ 

typedef struct dynamic_stack* Stack;
typedef void* any_t;

/******************** Stack functions declarations ********************/

// Returns a Stack instance.
Stack create_stack(void);

// Frees the space where the stack is located.
void destroy_stack(Stack stack);

// Checks if the stack is empty. Returns 1 if empty, 0 if not.
int stack_is_empty(const Stack stack);

// Pushes an element onto the stack
void stack_push(Stack stack, any_t value);

// Pops an element from the stack, and returns its value.
any_t stack_pop(Stack stack);

// Returns the value of the top element of the stack without removing it.
any_t stack_top(const Stack stack);

#endif // _STACK_H