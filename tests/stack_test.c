#include "adt/stack.h"
#include "testaux.h"

void test_new_stack(void) {
    test_start("Create a new Stack");

    stack_t *stack = stack_create(NULL);

    test_true(stack_is_empty(stack), "A newly created stack must be empty");
    
    stack_top(stack);
    test_errno_with(ENODATA, "You can't get the top of an empty stack");

    stack_pop(stack);
    test_errno_with(ENODATA, "You can't pop an element from an empty stack");

    stack_destroy(stack);

    test_end();
}

void test_one_element(void) {
    test_start("Push an element to the stack, look at its top, and then pop it");

    stack_t *stack = stack_create(NULL);
    int num = 13624;
    void *ptr = NULL;

    stack_push(stack, &num);

    test_false(stack_is_empty(stack), "The stack is not empty after storing an element");

    ptr = stack_top(stack);
    test_equals(ptr, &num, intcmp, "The element at the top of the stack must be its only element");
    ptr = NULL;
    
    test_false(stack_is_empty(stack), "The stack must have the element after only getting the top element");

    ptr = stack_pop(stack);
    test_equals(ptr, &num, intcmp, "The popped element was the only pushed one");

    test_true(stack_is_empty(stack), "The stack has to be empty after popping the only element that was pushed");

    stack_destroy(stack);

    test_end();
}

void test_lifo_property(void) {
    test_start("Check that the Stack structure follows the LIFO Property correctly");

    stack_t *stack = stack_create(NULL);
    int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    void *ptr;

    for (int i = 0 ; i < 10 ; i++) {
        stack_push(stack, &nums[i]);
    }

    for (int i = 10 - 1 ; i >= 0 ; i--) {
        test_false(stack_is_empty(stack), "Until there are no elements left in the stack, it must not be empty");
        ptr = stack_pop(stack);
        test_not_null(ptr, "There must be elements left to be popped");
        test_equals(ptr, &nums[i], intcmp, "The elements are popped in the inverted order as they were pushed");
    }

    stack_destroy(stack);

    test_end();
}

void test_empty_stack(void) {
    test_start("Push elements into the stack, pop them all, then check that the empty stack is the same as a newly created one");

    stack_t *stack = stack_create(NULL);
    int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for (int i = 0 ; i < 10 ; i++) {
        stack_push(stack, &nums[i]);
    }

    for (int i = 0 ; i < 10 ; i++) {
        stack_pop(stack);
    }

    test_true(stack_is_empty(stack), "After popping everything, the stack has to be empty");

    stack_top(stack);
    test_errno_with(ENODATA, "You can't get the top of an empty stack");

    stack_pop(stack);
    test_errno_with(ENODATA, "You can't pop an element from an empty stack");

    stack_destroy(stack);

    test_end();
}

void test_bulk(void) {
    test_start("The Stack must work with a lot of elements");

    stack_t *stack = stack_create(free);
    void *ptr = NULL;

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        int *val = (int *) malloc(sizeof(int));
        if (!val) { stack_destroy(stack); test_end(); return; }

        *val = i;
        stack_push(stack, val);
        ptr = stack_top(stack);
        test_not_null(ptr, "There must be elements left to be popped");
        test_equals(ptr, &i, intcmp, "The element just pushed is the new top element");
    }

    for (int i = BULK_AMOUNT - 1 ; i >= 0 ; i--) {
        ptr = stack_pop(stack);
        test_equals(ptr, &i, intcmp, "The element just popped is the correct one");
        free(ptr);
    }

    test_true(stack_is_empty(stack), "The stack is empty once again after popping a lot of elements");

    stack_destroy(stack);

    test_end();
}

void test_bulk_destroy(void) {
    test_start("The Stack frees the memory of its elements correctly with its destroy operation");

    stack_t *stack = stack_create(free);
    void *ptr = NULL;

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        int *val = (int *) malloc(sizeof(int));
        if (!val) { stack_destroy(stack); test_end(); return; }

        *val = i;
        stack_push(stack, val);
        ptr = stack_top(stack);
        test_not_null(ptr, "There must be elements left to be popped");
        test_equals(ptr, &i, intcmp, "The element just pushed is the new top element");
    }

    test_false(stack_is_empty(stack), "The stack is not empty before the call to the destroy operation");

    stack_destroy(stack);

    test_end();
}

void test_other_types(void) {
    test_start("Check that the Stack can store different datatypes");

    stack_t *string_stack = stack_create(NULL);
    stack_t *float_stack = stack_create(NULL);
    void *ptr;

    char *strings[] = {"hello world", "my name is", "goodbye world"};
    float floats[] = {3.14f, 2.5f, 6431.123f};

    for (int i = 0 ; i < 3 ; i++) {
        stack_push(string_stack, strings[i]);
    }
    for (int i = 3 - 1 ; i >= 0 ; i--) {
        test_false(stack_is_empty(string_stack), "Until there are no elements left in the stack, it must not be empty");
        ptr = stack_pop(string_stack);
        test_not_null(ptr, "There must be elements left to be popped");
        test_equals(ptr, strings[i], vstrcmp, "The elements are popped in the inverted order as they were pushed");
    }

    for (int i = 0 ; i < 3 ; i++) {
        stack_push(float_stack, &floats[i]);
    }
    for (int i = 3 - 1 ; i >= 0 ; i--) {
        test_false(stack_is_empty(float_stack), "Until there are no elements left in the stack, it must not be empty");
        ptr = stack_pop(float_stack);
        test_not_null(ptr, "There must be elements left to be popped");
        test_equals(ptr, &floats[i], floatcmp, "The elements are popped in the inverted order as they were pushed");
    }

    stack_destroy(string_stack);
    stack_destroy(float_stack);

    test_end();
}

int main(void) {
    test_new_stack();
    test_one_element();
    test_lifo_property();
    test_empty_stack();
    test_bulk();
    test_bulk_destroy();
    test_other_types();

    return 0;
}