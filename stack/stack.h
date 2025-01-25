#ifndef _STACK_H
#define _STACK_H

#include <stdbool.h>

/******************** Stack structure definition ********************/ 

typedef struct stack_t* Stack;

/******************** Stack functions declarations ********************/

/* Returns an empty stack. 

POST:
- If there is not enough memory to allocate the stack, NULL will be returned. */
Stack stack_create(void); 

/* Frees the memory were the stack is allocated. */
void stack_destroy(Stack);

/* Returns true if the stack is empty; otherwise, returns false. */
bool stack_is_empty(const Stack);

/* Pushes an element into the top of the stack. 

POST:
- Returns true if the action was completed successfully; otherwise, returns false. */
bool stack_push(Stack, void*);

/* Pops the element from the top of the stack and stores its value inside the generic 
pointer.

POST
- If the stack is empty or any other error happens, returns false; otherwise, returns
true */
bool stack_pop(Stack, void**);

/* Stores the value of the element on the top of the stack inside the pointer.

POST
- If the stack is empty or any other error happens, returns false; otherwise, returns
true */
bool stack_top(const Stack, void**);

#endif // _STACK_H