# ADT Stack

A simple data structure that follows the LIFO principle - Last In, First Out.

## Opreations

```c
/* Returns an instance of an empty stack

PRE:
- `elem_size` must be the size for the elements that will be stored in the stack. */
Stack stack_create(size_t elem_size);

/* Frees the memory where the stack is allocated */
void stack_destroy(Stack stack);

/* Add `elem`'s value into the top of the stack.

PRE:
- `sizeof(elem)` must be equal to the given when the stack was created. */
void stack_push(Stack stack, void* elem);

/* Delete and return the value of the element at the top of the stack. If the stack does not
have any element to pop, it must abort the flow. 

POST:
- The returned pointer should be freed after finished using it. */
void* stack_pop(Stack stack);

/* Return the value of the element at the top of the stack. If the stack does not have any 
element to pop, it must abort the flow. */
void* stack_top(const Stack stack);

/* Returns true if the stack is empty; otherwise, it returns false. */
bool stack_is_empty(const Stack stack);
```

## Compiling

To compile your code run the following line, where `source_file` is the `.c` that needs `stack.h`.

    gcc -o <output_file> <source_file> stack.c
