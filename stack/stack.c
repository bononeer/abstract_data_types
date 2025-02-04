#include <string.h>
#include <stdlib.h>
#include "stack.h"

#define INITIAL_SIZE 4
#define VARIATION_SIZE 2

/******************** structure definition ********************/ 

struct stack_t {
    void **data;
    size_t quantity;
    size_t capacity;
};

/******************** static functions declarations ********************/ 

/* For a given stack, changes the size of its intern data array for one of the length 
given. No element is deleted from the stack. Returns false if issue, true if not. */
static bool stack_resize(Stack stack, size_t new_size);

/******************** Stack operations definitions ********************/ 

Stack stack_create(void) {
    Stack stack = (Stack)malloc(sizeof(struct stack_t));
    if (stack == NULL) return NULL;

    stack->quantity = 0;
    stack->capacity = INITIAL_SIZE;

    void **data_temp = (void**)malloc(stack->capacity * sizeof(void*));
    if (data_temp == NULL) {
        free(stack);
        return NULL;
    }
    stack->data = data_temp;

    return stack;
}

void stack_destroy(Stack stack, void (*elem_destroy)(void* elem)) {
    if (elem_destroy == NULL) elem_destroy = free;
    for (size_t i = 0; i < stack->quantity; i++) {
        elem_destroy(stack->data[i]);
    }
    free(stack->data);
    free(stack);
}

bool stack_push(Stack stack, void* elem) {
    if (stack->quantity == stack->capacity) {
        bool ok = stack_resize(stack, stack->capacity * VARIATION_SIZE);
        if (!ok) return false;
    }

    void *new_elem = malloc(sizeof(void*));
    if (new_elem == NULL) return false;

    memcpy(new_elem, elem, sizeof(void*));
    stack->data[stack->quantity++] = new_elem;

    return true;
}

void* stack_pop(Stack stack) {
    if (stack_is_empty(stack)) return NULL;

    void* deleted = stack->data[--stack->quantity];

    if(stack->quantity * 2 * VARIATION_SIZE == stack->capacity) {
        bool ok = stack_resize(stack, stack->capacity / VARIATION_SIZE);
        if (!ok) return NULL;
    }

    return deleted;
}

void* stack_top(const Stack stack) {
    if (stack_is_empty(stack)) return NULL;

    return stack->data[stack->quantity-1];
}

bool stack_is_empty(const Stack stack) {
    return stack->quantity == 0;
}

/******************** static functions definitions ********************/

static bool stack_resize(Stack stack, size_t new_size) {
    void** temp_data = realloc(stack->data, new_size * sizeof(void*));
    if (temp_data == NULL) return false;

    stack->data = temp_data;
    stack->capacity = new_size;

    return true;
}