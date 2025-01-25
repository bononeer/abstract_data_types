#include <stdlib.h>
#include "stack.h"

#define INITIAL_SIZE 4
#define VARIATION_SIZE 2

/******************** static functions declarations ********************/ 

static bool stack_resize(Stack stack, size_t new_size);

/******************** structure definition ********************/ 
struct stack_t {
    void** data;
    size_t quantity;
    size_t capacity;
};

/******************** Stack functions definitions ********************/ 

Stack stack_create(void) {
    Stack stack = (Stack)malloc(sizeof(struct stack_t));
    if (!stack) {
        return NULL;
    }

    void** data = (void**)malloc(INITIAL_SIZE * sizeof(void*));
    if (!data) {
        free(stack);
        return NULL;
    }

    stack->data = data;
    stack->quantity = 0;
    stack->capacity = INITIAL_SIZE;

    return stack;
} 

void stack_destroy(Stack stack) {
    free(stack->data);
    free(stack);
}

bool stack_is_empty(const Stack stack) {
    return stack->quantity == 0;
}

bool stack_push(Stack stack, void* value) {
    bool success = true;

    if (stack->quantity == stack->capacity) {
        success = stack_resize(stack, stack->capacity * VARIATION_SIZE);
    }

    stack->data[stack->quantity] = value;
    stack->quantity++;

    return success;
}

bool stack_pop(Stack stack, void** ptr) {
    bool success = true;
    if (stack_is_empty(stack)) {
        return false;
    }

    *ptr = stack->data[--stack->quantity];

    if (stack->quantity*2*VARIATION_SIZE <= stack->capacity) {
        success = stack_resize(stack, stack->capacity / VARIATION_SIZE);
    }

    return success;
}

bool stack_top(const Stack stack, void** ptr) {
    if (stack_is_empty(stack)) {
        return false;
    }

    *ptr = stack->data[stack->quantity-1];

    return true;
}

/******************** static functions definitions ********************/ 

bool stack_resize(Stack stack, size_t new_size) {
    void** temp_data = realloc(stack->data, new_size * sizeof(void*));
    if (!temp_data) {
        return false;
    }

    stack->data = temp_data;
    stack->capacity = new_size;

    return true;
}