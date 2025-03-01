#include <stdlib.h>
#include "stack.h"

#define INITIAL_SIZE 4
#define VARIATION_SIZE 2

/******************** structure definition ********************/ 

typedef void (*elem_destroy)(void*);

struct stack_t {
    void **data;
    size_t quantity;
    size_t capacity;
    elem_destroy destroy;
};

/******************** static functions declarations ********************/ 

static bool stack_resize(Stack stack, size_t new_size);

/******************** Stack operations definitions ********************/ 

Stack stack_create(void (*elem_destroy)(void* elem)) {
    Stack stack = (Stack)malloc(sizeof(struct stack_t));
    if (stack == NULL) return NULL;

    stack->quantity = 0;
    stack->capacity = INITIAL_SIZE;
    stack->destroy = elem_destroy;

    void **data_temp = (void**)malloc(stack->capacity * sizeof(void*));
    if (data_temp == NULL) {
        free(stack);
        return NULL;
    }
    stack->data = data_temp;

    return stack;
}

void stack_destroy(Stack stack) {
    if (stack->destroy != NULL) for (size_t i = 0; i < stack->quantity; i++) (stack->destroy)(stack->data[i]);
    free(stack->data);
    free(stack);
}

bool stack_push(Stack stack, void* elem) {
    if (stack->quantity == stack->capacity) {
        bool ok = stack_resize(stack, stack->capacity * VARIATION_SIZE);
        if (!ok) return false;
    }

    stack->data[stack->quantity++] = elem;

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