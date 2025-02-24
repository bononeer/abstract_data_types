#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../queue/queue.h"
#include "assert_msg.h"

typedef struct {
    char* string;
    int integer;
} MyStruct;

static void print_test(bool, const char*);
static MyStruct* struct_create(char* string, int integer);
static void struct_destroy(void* value);

static void test_new_queue(void) {
    printf("TEST: A newly created queue works as expected.\n");

    Queue q = queue_create(NULL);

    print_test(q != NULL, "Create a new queue");
    print_test(queue_is_empty(q), "A newly created queue must be empty");

    print_test(queue_dequeue(q) == NULL, "An empty queue returns NULL if its tries to dequeue");
    print_test(queue_front(q) == NULL, "An empty queue returns NULL if its tries to peek at the front");

    queue_destroy(q);
}

static void test_destroy(void) {    // Check with valgrind
    printf("TEST: Check that the memeory of the elements in the queue are freed correctly when using the destroy operation\n");

    Queue q1 = queue_create(NULL);
    int num = 2;

    print_test(queue_enqueue(q1, &num), "The element was enqueued correctly"); // No allocs for element's memory

    queue_destroy(q1);

    Queue q2 = queue_create(free);
    int* another_num = (int*)malloc(sizeof(int));
    print_test(another_num != NULL, "");
    *another_num = 3;

    print_test(queue_enqueue(q2, another_num), "The element was enqueued correctly"); // Element with allocated memory

    queue_destroy(q2);
}

static void test_queue_one_element(void) {
    printf("TEST: Tries the flow of enqueuing one element, peeking at the front and dequeuing it out of the queue.\n");

    Queue q = queue_create(NULL);
    void* front = NULL;
    float pi = 3.14f;

    queue_enqueue(q, &pi);
    print_test(!queue_is_empty(q), "A queue with an element is not empty");
    
    front = queue_front(q);
    print_test(*(float*)front == pi, "The element at the front of the queue is the one just enqueued");
    print_test(!queue_is_empty(q), "Peeking at the element in front of the queue does not dequeue it");
    
    front = queue_dequeue(q);
    print_test(*(float*)front == pi, "The element dequeued is the one that was enqueued before");
    print_test(queue_is_empty(q), "After dequeuing its only element, the queue is empty");

    print_test(queue_dequeue(q) == NULL, "After dequeuing its only element, there are no more elements left to dequeue");

    queue_destroy(q);
}

static void test_fifo(void) {
    printf("TEST: Verifies that the queue follows the FIFO principle correctly\n");

    Queue q = queue_create(NULL);
    void* front = NULL;
    int elements[3] = {99, 14, 100};
    char* suffixes[3] = {"st", "nd", "rd"};
    char test_msg[60];

    for (int i = 0 ; i < 3 ; i++) {
        queue_enqueue(q, &elements[i]);
        print_test(!queue_is_empty(q), "The queue must not be empty after enqueing elements");
    }

    for (int i = 0 ; !queue_is_empty(q) ; i++) {
        sprintf(test_msg, "The %d%s element dequeued is the %d%s enqueued", i+1, suffixes[i], i+1, suffixes[i]);
        front = queue_dequeue(q);
        print_test(*(int*)front == elements[i], test_msg);
    }

    queue_destroy(q);
}

void test_bulk_fifo(void) {
    printf("TEST: Verifies that the queue follows the FIFO principle correctly even when working with lots of elements\n");

    Queue q = queue_create(free);
    void* front = NULL;
    int* first = (int*)malloc(sizeof(int));
    print_test(first != NULL, "");
    *first = 0;

    queue_enqueue(q, first);
    for (int i = 1 ; i < BULK_AMOUNT ; i++) {
        int* value = (int*)malloc(sizeof(int));
        print_test(value != NULL, "");
        *value = i;
        queue_enqueue(q, value);
        front = queue_front(q);
        print_test(*(int*)front == *first, "Enqueuing more elements will not change the front of the queue");
    }

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        front = queue_dequeue(q);
        print_test(*(int*)front == i, "The elements are dequeued in the same order as they were added");
        free(front);
    }
    print_test(queue_is_empty(q), "After dequeuing all the elements, the queue is empty");

    queue_destroy(q);
}

void test_emptied_queue(void) {
    printf("TEST: Verifies that an emptied queue acts like a newly created queue\n");

    Queue q = queue_create(free);
    bool ok = true;
    void* front = NULL;
    int* first = (int*)malloc(sizeof(int));
    if (first == NULL) return;
    *first = 0;

    queue_enqueue(q, first);
    for (int i = 1 ; i < AMOUNT && q ; i++) {
        int* value = (int*)malloc(sizeof(int));
        print_test(value != NULL, "");
        *value = i;
        queue_enqueue(q, value);
        front = queue_front(q);
        ok = *(int*)front != i && !queue_is_empty(q);
    }
    print_test(q, "A good amount of elements can be enqueued into the queue");

    for (int i = 0 ; i < AMOUNT && ok ; i++) {
        front = queue_dequeue(q);
        ok = *(int*)front == i;
        free(front);
    }
    print_test(queue_is_empty(q), "An emptied queue must be empty");

    print_test(queue_dequeue(q) == NULL, "An empty queue returns NULL if its tries to dequeue");
    print_test(queue_front(q) == NULL, "An empty queue returns NULL if its tries to peek at the front");

    queue_destroy(q);
}

void test_struct_values(void) {
    printf("TEST: Enqueue structs into the queue and check that it works correctly\n");

    Queue q = queue_create(*struct_destroy);
    char *string1 = "Hello World", *string2 = "This is a struct!"; 
    int integer1 = 10, integer2 = -14;
    MyStruct *my_struct1 = struct_create(string1, integer1), *my_struct2 = struct_create(string2, integer2);
    MyStruct* ptr = NULL;

    print_test(q != NULL, "The queue is created correctly with a specific destroy function");

    print_test(queue_is_empty(q), "The queue must be empty before enqueuing any struct");
    print_test(queue_enqueue(q, my_struct1), "The struct can be enqueued correctly");
    print_test(queue_enqueue(q, my_struct2), "The struct can be enqueued correctly");
    print_test(!queue_is_empty(q), "The queue must not be empty after enqueuing the structs");

    ptr = (MyStruct*)queue_dequeue(q);
    print_test(strcmp(ptr->string, string1) == 0, "The struct that was in the first position is the first one added");
    print_test(ptr->integer == integer1, "The struct data was stored correctly");
    struct_destroy(ptr);
    ptr = NULL;

    print_test(!queue_is_empty(q), "There must still be a struct left");
    
    ptr = (MyStruct*)queue_front(q);
    print_test(!queue_is_empty(q), "Peeking at the front of a queue with struct types does not make it empty");
    print_test(strcmp(ptr->string, string2) == 0, "The new struct at the front is the last one added");
    print_test(ptr->integer == integer2, "The struct data was stored correctly");

    queue_destroy(q);
}

int main(void) {
    test_new_queue();
    test_destroy();
    test_queue_one_element();
    test_fifo();
    test_bulk_fifo();
    test_emptied_queue();
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