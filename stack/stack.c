#include <string.h>
#include "stack.h"
#include "../assert_msg.h"

#define INITIAL_SIZE 4
#define VARIATION_SIZE 2
#define EMPTY_STACK_ERR "The stack is empty"
#define MEM_ELEM_ERR "Not enough memory for the element"

/******************** structure definition ********************/ 

struct stack_t {
    void **data;
    size_t quantity;
    size_t capacity;
};

/******************** static functions declarations ********************/ 

static void stack_resize(Stack, size_t);

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

void stack_destroy(Stack stack) {
    for (size_t i = 0; i < stack->quantity; i++) {
        free(stack->data[i]);
    }
    free(stack->data);
    free(stack);
}

void stack_push(Stack stack, void* elem) {
    if (stack->quantity == stack->capacity) {
        stack_resize(stack, stack->capacity * VARIATION_SIZE);
    }

    void *new_elem = (void*)malloc(sizeof(void*));
    assert_msg(new_elem != NULL, MEM_ELEM_ERR);

    memcpy(new_elem, elem, sizeof(void*));
    stack->data[stack->quantity++] = new_elem;
}

void* stack_pop(Stack stack) {
    assert_msg(!stack_is_empty(stack), EMPTY_STACK_ERR);

    void *deleted = malloc(sizeof(void*));
    assert_msg(deleted != NULL, MEM_ELEM_ERR);

    memcpy(deleted, stack->data[stack->quantity-1], sizeof(void*));
    free(stack->data[--stack->quantity]);

    if(stack->quantity * 2 * VARIATION_SIZE == stack->capacity) {
        stack_resize(stack, stack->capacity / VARIATION_SIZE);
    }

    return deleted;
}

void* stack_top(const Stack stack) {
    assert_msg(!stack_is_empty(stack), EMPTY_STACK_ERR);

    return stack->data[stack->quantity-1];
}

bool stack_is_empty(const Stack stack) {
    return stack->quantity == 0;
}

/******************** static functions definitions ********************/

static void stack_resize(Stack stack, size_t new_size) {
    void** temp_data = realloc(stack->data, new_size * sizeof(void*));
    assert_msg(temp_data != NULL, MEM_ELEM_ERR);

    stack->data = temp_data;
    stack->capacity = new_size;
}