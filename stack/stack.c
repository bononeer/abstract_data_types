#include <stdlib.h>
#include "stack.h"

#define INITIAL_CAPACITY 4
#define VARIATION_CAPACITY 2

/******************** structure definition ********************/ 

struct stack_t {
    void **data;
    size_t quantity;
    size_t capacity;
    destroy_func_t destroy;
};

/******************** static functions declarations ********************/ 

static bool stack_resize(Stack stack, size_t new_capacity);

/******************** Stack operations definitions ********************/ 

Stack stack_create(destroy_func_t elem_destroy) {
    Stack stack = (Stack)malloc(sizeof(struct stack_t));
    if (stack == NULL) return NULL;

    stack->data = (void**)malloc(INITIAL_CAPACITY * sizeof(void*));
    if (stack->data == NULL) {
        free(stack);
        return NULL;
    }
    stack->quantity = 0;
    stack->capacity = INITIAL_CAPACITY;
    stack->destroy = elem_destroy;

    return stack;
}

void stack_destroy(Stack stack) {
    if (stack == NULL) return;

    if (stack->destroy != NULL) for (size_t i = 0; i < stack->quantity; i++) (stack->destroy)(stack->data[i]);
    free(stack->data);
    free(stack);
}

bool stack_is_empty(const Stack stack) {
    return stack != NULL && stack->quantity == 0;
}

bool stack_push(Stack stack, void *elem) {
    if (stack == NULL) return false;
    if (stack->quantity == stack->capacity) if (!stack_resize(stack, stack->capacity * VARIATION_CAPACITY)) return false;

    stack->data[stack->quantity++] = elem;

    return true;
}

void *stack_top(const Stack stack) {
    if (stack == NULL || stack_is_empty(stack)) return NULL;

    return stack->data[stack->quantity-1];
}

void *stack_pop(Stack stack) {
    if (stack == NULL || stack_is_empty(stack)) return NULL;

    void *deleted = stack->data[--stack->quantity];
    if (stack->quantity * 2 * VARIATION_CAPACITY == stack->capacity) if (!stack_resize(stack, stack->capacity / VARIATION_CAPACITY)) return NULL;

    return deleted;
}

/******************** static functions definitions ********************/

static bool stack_resize(Stack stack, size_t new_capacity) {
    stack->data = realloc(stack->data, new_capacity * sizeof(void*));
    if (stack->data == NULL) return false;

    stack->capacity = new_capacity;

    return true;
}