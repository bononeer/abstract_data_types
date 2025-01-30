#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "../assert_msg.h"

#define AMOUNT 50
#define BULK_AMOUNT 10000

static void print_test(bool, const char*);

static void test_new_stack(void) {
    printf("TEST: A newly created stack works as expected.\n");

    Stack s = stack_create();

    print_test(s != NULL, "Create a new stack");
    print_test(stack_is_empty(s), "A newly created stack must be empty");

    stack_destroy(s);
}

static void test_stack_one_element(void) {
    printf("TEST: Tries the flow of pushing one element, peeking at the top and popping it out of the stack.\n");

    Stack s = stack_create();

    int num = 44;
    void* top;

    stack_push(s, &num);
    print_test(!stack_is_empty(s), "A stack with an element is not empty");
    top = stack_top(s);
    print_test(*(int*)top == num, "The element at the top of the stack has the same value as the one pushed before");
    top = NULL;
    top = stack_pop(s);
    print_test(*(int*)top == num, "The element popped from the top has the same value as the one pushed before");
    print_test(stack_is_empty(s), "The stack is empty after its only element was popped from it.");

    free(top);
    stack_destroy(s);
}

static void test_lifo(void) {
    printf("TEST: Verifies that the stack follows the LIFO principle correctly\n");

    Stack s = stack_create();

    float elements[3] = {0.112358f, 3.14f, 2.71f};
    char test_msg[60];
    char* suffixes[3] = {"st", "nd", "rd"};
    void* top = NULL;

    for (int i = 0 ; i < 3 ; i++) {
        stack_push(s, &elements[i]);
        print_test(!stack_is_empty(s), "The stack must not be empty after pushing elements");
    }

    for (int i = 2; !stack_is_empty(s); i--) {
        sprintf(test_msg, "The %d%s element out is the %d%s pushed", 3-i, suffixes[2-i], i+1, suffixes[i]);
        free(top);
        top = stack_pop(s);
        print_test(*(float*)top == elements[i], test_msg);
    }

    free(top);
    stack_destroy(s);
}

static void test_bulk_lifo(void) {
    printf("TEST: Verifies that the stack follows the LIFO principle correctly even when working with lots of elements\n");

    Stack s = stack_create();
    void* top = NULL;

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        stack_push(s, &i);
        top = stack_top(s);
        print_test(*(int*)top == i, "The current top of the stack is the just pushed one");
        print_test(!stack_is_empty(s), "The stack must not be empty when you keep pushing elements");
    }

    for (int i = BULK_AMOUNT - 1 ; i >= 0 ; i--) {
        top = stack_pop(s);
        print_test(*(int*)top == i, "The elements of the stack must be popped in the opposite way to which they were pushed");
        free(top);
    }
    print_test(stack_is_empty(s), "After popping all the elements the stack must be empty");

    stack_destroy(s);
}

static void test_change_pushed_value(void) {
    printf("TEST: Verifies that if the value of a variable that was pushed into a stack is changed, the value inside the stack doesn't change\n");

    Stack s = stack_create();

    char elem = 'a';
    stack_push(s, &elem);
    
    void* top = NULL;
    top = stack_top(s);
    print_test(*(char*)top == elem, "The element at the top is the one pushed");
    elem = 'b';
    top = stack_top(s);
    print_test(*(char*)top != elem, "After changing the value of the element that was pushed into the stack, the top of the stack didn't change");

    stack_destroy(s);
}

static void test_charge_stack(void) {
    printf("TEST: Pushes some elements into the stack, pops all of them, and charges it back again to show it works like a newly created stack\n");

    Stack s = stack_create();
    bool ok = true;
    void* top = NULL;

    for(int i = 0 ; i < AMOUNT && ok ; i++) {
        stack_push(s, &i);
        top = stack_top(s);
        ok = *(int*)top == i && !stack_is_empty(s);
    }
    print_test(ok, "A good amount of elements can be pushed into the stack");
    
    ok = true;
    for(int i = AMOUNT-1 ; i >= 0 && ok ; i--) {
        void* top = stack_pop(s);
        ok = *(int*)top == i;
        free(top);
    }
    print_test(stack_is_empty(s), "After every element was popped from the stack, it should be empty");
    print_test(ok, "The elements of the stack are popped in the opposite order to which they were pushed");

    for(int i = 0 ; i < AMOUNT && ok ; i++) {
        stack_push(s, &i);
        top = stack_top(s);
        ok = *(int*)top == i && !stack_is_empty(s);
    }
    print_test(ok, "After popping every element, the stack can still push a good amount of elements");

    stack_destroy(s);
}

static void test_bulk_charge_stack(void) {
    printf("TEST: Pushes a lot of elements into the stack, pops all of them, and charges it back again to check if the stack works correctly with lots of elements\n");

    Stack s = stack_create();
    bool ok = true;
    void* top = NULL;

    for(int i = 0 ; i < BULK_AMOUNT && ok ; i++) {
        stack_push(s, &i);
        top = stack_top(s);
        ok = *(int*)top == i && !stack_is_empty(s);
    }
    print_test(ok, "A good amount of elements can be pushed into the stack");
    
    ok = true;
    for(int i = BULK_AMOUNT-1 ; i >= 0 && ok ; i--) {
        void* top = stack_pop(s);
        ok = *(int*)top == i;
        free(top);
    }
    print_test(stack_is_empty(s), "After every element was popped from the stack, it should be empty");
    print_test(ok, "The elements of the stack are popped in the opposite order to which they were pushed");

    for(int i = 0 ; i < BULK_AMOUNT && ok ; i++) {
        stack_push(s, &i);
        top = stack_top(s);
        ok = *(int*)top == i && !stack_is_empty(s);
    }
    print_test(ok, "After popping every element, the stack can still push a good amount of elements");

    stack_destroy(s);
}

void test_push_pop(void) {
    printf("TEST: Pushes an element into the stack, pops it and repeats a good amount of times\n");

    Stack s = stack_create();
    void* top = NULL;
    int num = 2;

    stack_push(s, &num);
    for (int i = 0 ; i < AMOUNT ; i++) {
        top = stack_pop(s);
        int int_top = *(int*)top; 
        print_test(int_top == num, "The element popped must be the one pushed");
        stack_push(s, &int_top);
        free(top);
    }

    stack_destroy(s);
}

int main(void) {
    test_new_stack();
    test_stack_one_element();
    test_lifo();
    test_bulk_lifo();
    test_change_pushed_value();
    test_charge_stack();
    test_bulk_charge_stack();
    test_push_pop();
    
    return 0;
}

void print_test(bool success, const char* msg) {
    char result[10 + (int)strlen(msg)];
    sprintf(result, "FAIL: %s\n", msg);
    assert_msg(success, result);
}