#include "stack_aux.h"

#define INIT_CAPACITY 7
#define RESZ_CAPACITY 2

/* ############################### Structure ################################ */

struct dynamic_stack {
    void **data;
    size_t quantity;
    size_t capacity;
    destroy_func_t destroy;
};

/* ############################### Operations ############################### */

stack_t *stack_create(destroy_func_t elem_destroy) {
    stack_t *stack = (stack_t *) malloc(sizeof(struct dynamic_stack));
    if (err_no_mem(stack)) return NULL;

    stack->data = (void **) malloc(INIT_CAPACITY * sizeof(void *));
    if (err_no_mem(stack->data)) {
        free(stack);
        return NULL;
    }

    stack->quantity = 0;
    stack->capacity = INIT_CAPACITY;
    stack->destroy = elem_destroy;

    return stack;
}

void stack_destroy(void *ptr) {
    if (err_null_ptr(ptr)) return;

    stack_t *stack = (stack_t *) ptr;

    if (stack->destroy) for (size_t i = 0 ; i < stack->quantity ; i++) stack->destroy(*(stack->data + i)); 

    free(stack->data);
    free(stack);
}

bool stack_is_empty(const stack_t *stack) {
    return !err_null_ptr((stack_t *) stack) && stack->quantity == 0;
}

void stack_push(stack_t *stack, void *elem) {
    if (err_null_ptr(stack)) return;

    if (stack->quantity == stack->capacity && !stack_resize(stack, stack->capacity * RESZ_CAPACITY)) return;

    *(stack->data + stack->quantity++) = elem;
}

void *stack_pop(stack_t *stack) {
    if (err_empty_stack(stack)) return NULL;

    void *removed = *(stack->data + --stack->quantity);

    if (stack->quantity * 2 * RESZ_CAPACITY == stack->capacity && !stack_resize(stack, stack->capacity / RESZ_CAPACITY)) return NULL;

    return removed;
}

void *stack_top(const stack_t *stack) {
    if (err_empty_stack((stack_t *) stack)) return NULL;

    return *(stack->data + stack->quantity - 1);
}

/* ############################# Aux Operations ############################# */

bool stack_resize(stack_t *stack, size_t new_capacity) {
    stack->data = (void **) realloc(stack->data, new_capacity * sizeof(void *));
    if (err_no_mem(stack->data)) return false;

    stack->capacity = new_capacity;

    return true;
}

bool err_empty_stack(stack_t *stack) {
    if (err_null_ptr(stack)) return true;
    if (!stack_is_empty(stack)) return false;

    errno = ENODATA;
    return true;
}