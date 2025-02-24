#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stack/stack.h"
#include "assert_msg.h"

typedef struct {
    char* string;
    int integer;
} MyStruct;

static void print_test(bool, const char*);
static MyStruct* struct_create(char* string, int integer);
static void struct_destroy(void* value);

static void test_new_stack(void) {
    printf("TEST: A newly created stack works as expected.\n");

    Stack s = stack_create(NULL);

    print_test(s != NULL, "Create a new stack");
    print_test(stack_is_empty(s), "A newly created stack must be empty");

    print_test(stack_pop(s) == NULL, "An empty stack returns NULL if its tries to pop");
    print_test(stack_top(s) == NULL, "An empty stack returns NULL if its tries to peek at the top");

    stack_destroy(s);
}

static void test_destroy(void) {    // Check with valgrind
    printf("TEST: Check that the memeory of the elements in the stack are freed correctly when using the destroy operation\n");

    Stack s1 = stack_create(NULL);
    int num = 2;

    print_test(stack_push(s1, &num), "The element was pushed correctly"); // No allocs for element's memory

    stack_destroy(s1);

    Stack s2 = stack_create(free);
    int* another_num = (int*)malloc(sizeof(int));
    print_test(another_num != NULL, "");
    *another_num = 3;

    print_test(stack_push(s2, another_num), "The element was pushed correctly"); // Element with allocated memory
    
    stack_destroy(s2);
}

static void test_stack_one_element(void) {
    printf("TEST: Tries the flow of pushing one element, peeking at the top and popping it out of the stack.\n");

    Stack s = stack_create(NULL);
    void* top = NULL;
    int num = 44;

    stack_push(s, &num);
    print_test(!stack_is_empty(s), "A stack with an element is not empty");

    top = stack_top(s);
    print_test(*(int*)top == num, "The element at the top of the stack has the same value as the one pushed before");
    print_test(!stack_is_empty(s), "Peeking at the top of the stack doesn't pop it");

    top = stack_pop(s);
    print_test(*(int*)top == num, "The element popped from the top has the same value as the one pushed before");
    print_test(stack_is_empty(s), "The stack is empty after its only element was popped from it.");

    print_test(stack_pop(s) == NULL, "After popping its last element, there are no elements left in the stack");

    stack_destroy(s);
}

static void test_lifo(void) {
    printf("TEST: Verifies that the stack follows the LIFO principle correctly\n");

    Stack s = stack_create(NULL);
    void* top = NULL;
    float elements[3] = {0.112358f, 3.14f, 2.71f};
    char* suffixes[3] = {"st", "nd", "rd"};
    char test_msg[60];

    for (int i = 0 ; i < 3 ; i++) {
        stack_push(s, &elements[i]);
        print_test(!stack_is_empty(s), "The stack must not be empty after pushing elements");
    }

    for (int i = 2; !stack_is_empty(s); i--) {
        sprintf(test_msg, "The %d%s element out is the %d%s pushed", 3-i, suffixes[2-i], i+1, suffixes[i]);
        top = stack_pop(s);
        print_test(*(float*)top == elements[i], test_msg);
    }

    stack_destroy(s);
}

static void test_bulk_lifo(void) {
    printf("TEST: Verifies that the stack follows the LIFO principle correctly even when working with lots of elements\n");

    Stack s = stack_create(free);
    void* top = NULL;

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        int* value = (int*)malloc(sizeof(int));
        print_test(value != NULL, "");
        *value = i;
        stack_push(s, value);
        top = stack_top(s);
        print_test(*(int*)top == i, "The current top of the stack is the just pushed one");
    }

    for (int i = BULK_AMOUNT - 1 ; i >= 0 ; i--) {
        top = stack_pop(s);
        print_test(*(int*)top == i, "The elements of the stack must be popped in the opposite way to which they were pushed");
        free(top);
    }
    print_test(stack_is_empty(s), "After popping all the elements the stack must be empty");

    stack_destroy(s);
}

static void test_emptied_stack(void) {
    printf("TEST: Verifies that an emptied stack acts like a newly created stack\n");

    Stack s = stack_create(free);
    void* top = NULL;
    bool ok = true;

    for(int i = 0 ; i < AMOUNT && ok ; i++) {
        int* value = (int*)malloc(sizeof(int));
        print_test(value != NULL, "");
        *value = i;
        stack_push(s, value);
        top = stack_top(s);
        ok = *(int*)top == i && !stack_is_empty(s);
    }
    print_test(ok, "A good amount of elements can be pushed into the stack");

    for(int i = AMOUNT-1 ; i >= 0 && ok ; i--) {
        top = stack_pop(s);
        ok = *(int*)top == i;
        free(top);
    }

    print_test(stack_is_empty(s), "The stack must be empty");

    print_test(stack_pop(s) == NULL, "An empty stack returns NULL if its tries to pop");
    print_test(stack_top(s) == NULL, "An empty stack returns NULL if its tries to peek at the top");

    stack_destroy(s);
}

void test_struct_values(void) {
    printf("TEST: Push structs into the stack and check that it works correctly\n");

    Stack s = stack_create(*struct_destroy);
    char *string1 = "Hello World", *string2 = "This is a struct!"; 
    int integer1 = 10, integer2 = -14;
    MyStruct *my_struct1 = struct_create(string1, integer1), *my_struct2 = struct_create(string2, integer2);
    MyStruct* ptr = NULL;

    print_test(s != NULL, "The stack is created correctly with a specific destroy function");

    print_test(stack_is_empty(s), "The stack must be empty before pushing any struct");
    print_test(stack_push(s, my_struct1), "The struct can be pushed correctly");
    print_test(stack_push(s, my_struct2), "The struct can be pushed correctly");
    print_test(!stack_is_empty(s), "The stack must not be empty after pushing the structs");

    ptr = (MyStruct*)stack_pop(s);
    print_test(strcmp(ptr->string, string2) == 0, "The struct that was stored at the top is the last one added");
    print_test(ptr->integer == integer2, "The struct data was stored correctly");
    struct_destroy(ptr);
    ptr = NULL;

    print_test(!stack_is_empty(s), "There must still be a struct left");
    
    ptr = (MyStruct*)stack_top(s);
    print_test(!stack_is_empty(s), "Peeking at the top element of a stack with struct types does not make it empty");
    print_test(strcmp(ptr->string, string1) == 0, "The new struct at the top is the first one added");
    print_test(ptr->integer == integer1, "The struct data was stored correctly");

    stack_destroy(s);
}

int main(void) {
    test_new_stack();
    test_destroy();
    test_stack_one_element();
    test_lifo();
    test_bulk_lifo();
    test_emptied_stack();
    test_struct_values();
    
    return 0;
}

void print_test(bool success, const char* msg) {
    char result[10 + (int)strlen(msg)];
    sprintf(result, "FAIL: %s\n", msg);
    assert_msg(success, result);
}

MyStruct* struct_create(char* string, int integer) {
    MyStruct* my_struct = (MyStruct*)malloc(sizeof(MyStruct));
    if (my_struct == NULL) return NULL;

    my_struct->string = (char*)malloc((strlen(string)+1) * sizeof(char));
    if (my_struct->string == NULL) {
        free(my_struct);
        return NULL;
    }
    strcpy(my_struct->string, string);

    my_struct->integer = integer;

    return my_struct;
}

void struct_destroy(void* value) {
    if (value != NULL) {
        MyStruct* my_struct = (MyStruct*)value;
        free(my_struct->string);
        free(my_struct);
    }
}