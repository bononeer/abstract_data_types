#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

#define INITIAL_SIZE 4
#define VARIATION_SIZE 2
#define EMPTY_MSG "The stack is empty\n"
#define RESIZE_MSG "Failed capacity resize\n"

bool _error_empty_stack(stack_t* stack);
bool _stack_resize(stack_t* stack, size_t new_size);

typedef unsigned long size_t;

struct _Stack {
    void** data;
    size_t quantity;
    size_t capacity;
};

stack_t* create_stack() {
    stack_t* stack = malloc(sizeof(stack_t));
    if (!stack) return NULL;

    void** data = malloc(INITIAL_SIZE * sizeof(void*));
    if (!data) {
        free(stack);
        return NULL;
    }

    stack->capacity = INITIAL_SIZE;
    stack->quantity = 0;
    stack->data = data;

    return stack;
}

void destroy_stack(stack_t* stack) {
    free(stack->data);
    free(stack);
}

bool stack_is_empty(const stack_t* stack) {
    return stack->quantity == 0;
}

bool stack_push(stack_t* stack, void* value) {
    bool err = false;

    if (stack->quantity == stack->capacity) {
        err = _stack_resize(stack, stack->capacity/VARIATION_SIZE);
    }
    if (err) return false;

    stack->data[stack->quantity] = value;
    stack->quantity++;

    return true;
}

void* stack_pop(stack_t* stack) {
    bool err = false;
    
    err = _error_empty_stack(stack);
    if (err) return NULL;

    void* deleted = stack->data[stack->quantity-1];

    if (stack->quantity*4 <= stack->capacity) {
        err = _stack_resize(stack, stack->capacity/VARIATION_SIZE);
    }
    if (err) return NULL;

    stack->quantity--;

    return deleted;
}

void* stack_peek(stack_t* stack) {
    bool err = false;
    
    err = _error_empty_stack(stack);
    if (err) return NULL;

    return stack->data[stack->quantity-1];
}

bool _error_empty_stack(stack_t* stack) {
    if (stack_is_empty(stack)) {
        fprintf(stderr, EMPTY_MSG);
        destroy_stack(stack);
        
        return true;
    }
    return false;
}

bool _stack_resize(stack_t* stack, size_t new_size) {
    void** temp_data = realloc(stack->data, new_size);
    if (!temp_data) {
        fprintf(stderr, RESIZE_MSG);
        destroy_stack(stack);
        
        return true;
    }

    stack->data = temp_data;

    return false;
}