# Abstract Data Types

Interface and implementations for ADT in C language.

## Compile 

using gcc (extra flags for error detection):

    > gcc -Wall -Werror -Wextra -std=c99 -pedantic main.c <adt>.c -o main

## ADT Stack

A Data Structure that follows the LIFO (Last-in, First-out) principle.<br>
You can use the following functions when including "stack.h":

```c
typedef struct dynamic_stack* Stack;
typedef void* any_t;

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
```
