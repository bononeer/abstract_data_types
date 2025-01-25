#include <stdio.h>
#include "stack.h"

static void print_test(bool, const char*);

static void test_new_stack(void) {
    printf("\nTEST: A newly created stack works as expected.\n");

    Stack s = stack_create();

    print_test(s != NULL, "Create a new stack");
    print_test(stack_is_empty(s), "A newly created stack must be empty");

    void* top;
    print_test(!stack_top(s, &top), "Peek at the top of an empty stack should fail");
    print_test(!stack_pop(s, &top), "Pop the top of an empty stack should fail");
}

static void test_push_elements(void) {
    printf("\nTEST: Push an elements to the top of the stack.\n");

    Stack s = stack_create();

    int num1 = 44, num2 = 19;
    void* top;
    print_test(stack_push(s, &num1), "Push an integer to the top of an empty stack");
    print_test(!stack_is_empty(s), "A stack with an element is not empty");
    stack_top(s, &top);
    print_test(*(int*)top == num1, "The element at the top of the stack has the same value as the one pushed before");
    print_test(stack_push(s, &num2), "Push an integer to the top of a non-empty stack");
    stack_top(s, &top);
    print_test(*(int*)top == num2, "The element at the top of the stack has the same value as the last pushed one");
    print_test(*(int*)top != num1, "The elemnt at the top is no longer the one before");
    print_test(!stack_is_empty(s), "A stack with more than one element is not empty");
}

static void test_peek_top(void) {
    printf("\nTEST: Peek at the top of a stack.\n");

    Stack s = stack_create();
}

int main(void) {
    test_new_stack();
    test_push_elements();
    test_peek_top();

    return 0;
}

void print_test(bool success, const char* msg) {
    char* result = success ? "OK" : "ERROR";    
    printf("%s... %s\n", msg, result);
}