#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

#define INITIAL_SIZE 4
#define VARIATION_SIZE 2

#define INIT_MEMORY_ERR "Not enough memory to initialize Stack.\n"
#define DESTROY_ERR "Cannot destroy Stack.\n"
#define EMPTY_ERR "The Stack is empty.\n"
#define RESIZE_ERR "Not enooug memory to resize Stack.\n"

/******************** static functions declarations ********************/ 

static void stack_empty_error(const Stack stack);
static void stack_resize(Stack stack, size_t new_size);

/******************** dynamic_stack structure definition ********************/ 

struct dynamic_stack {
    any_t* data;
    size_t quantity;
    size_t capacity;
};

/******************** Stack functions definitions ********************/ 

Stack create_stack(void) {
    Stack stack = (Stack)malloc(sizeof(struct dynamic_stack));
    if (!stack) {
        fprintf(stderr, INIT_MEMORY_ERR);
        abort();
    }

    any_t *data = (any_t*)malloc(INITIAL_SIZE * sizeof(any_t));
    if (!data) {
        fprintf(stderr, INIT_MEMORY_ERR);
        abort();
    }

    stack->data = data;
    stack->quantity = 0;
    stack->capacity = INITIAL_SIZE;

    return stack;
}

void destroy_stack(Stack stack) {
    if (!stack) {
        fprintf(stderr, DESTROY_ERR);
        abort();
    }

    free(stack->data);
    free(stack);
}

int stack_is_empty(const Stack stack) {
    return !stack->quantity;
}

void stack_push(Stack stack, any_t value) {
    if (stack->quantity == stack->capacity) {
        stack_resize(stack, stack->capacity * VARIATION_SIZE);
    }

    stack->data[stack->quantity++] = value;
}

any_t stack_pop(Stack stack) {
    stack_empty_error(stack);

    any_t top_item = stack->data[--stack->quantity];

    if (stack->quantity*4 <= stack->capacity) {
        stack_resize(stack, stack->capacity / VARIATION_SIZE);
    }

    return top_item;
}

any_t stack_top(const Stack stack) {
    stack_empty_error(stack);

    return stack->data[stack->quantity-1];
}

/******************** static functions definitions ********************/ 

static void stack_empty_error(const Stack stack) {
    if (stack_is_empty(stack)) {
        fprintf(stderr, EMPTY_ERR);
        abort();
    }
}

static void stack_resize(Stack stack, size_t new_size) {
    any_t* temp_data = realloc(stack->data, new_size * sizeof(any_t));
    if (!temp_data) {
        fprintf(stderr, RESIZE_ERR);
        abort();
    }

    stack->data = temp_data;
    stack->capacity = new_size;
}