#include <stdbool.h>

typedef struct _Stack stack_t;

/* Returns an instance of a stack. */
stack_t* create_stack();
 
/* Frees the memory where the stack is located. */
void destroy_stack(stack_t* stack);

/* Returns true if `stack` has no values stacked. Returns false in any other case. */
bool stack_is_empty(const stack_t* stack);

/* Pushes `value` on the top of the stack. Returns void. */
bool stack_push(stack_t* stack, void* value);

/* Pops the element on the top of the stack and returns its value. */
void* stack_pop(stack_t* stack);

/* Returns the value of the element on top of the stack. */
void* stack_peek(stack_t* stack);