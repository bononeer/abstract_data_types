#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../queue/queue.h"
#include "assert_msg.h"

#define AMOUNT 50
#define BULK_AMOUNT 10000

static void print_test(bool, const char*);

static void test_new_queue(void) {
    printf("TEST: A newly created queue works as expected.\n");

    Queue q = queue_create();

    print_test(q != NULL, "Create a new queue");
    print_test(queue_is_empty(q), "A newly created queue must be empty");

    print_test(queue_dequeue(q) == NULL, "An empty queue returns NULL if its tries to dequeue");
    print_test(queue_front(q) == NULL, "An empty queue returns NULL if its tries to peek at the front");

    queue_destroy(q, NULL);
}

static void test_queue_one_element(void) {
    printf("TEST: Tries the flow of enqueuing one element, peeking at the front and dequeuing it out of the queue.\n");

    Queue q = queue_create();
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

    free(front);
    queue_destroy(q, NULL);
}

static void test_fifo(void) {
    printf("TEST: Verifies that the queue follows the FIFO principle correctly\n");

    Queue q = queue_create();
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
        free(front);
    }

    queue_destroy(q, NULL);
}

void test_bulk_fifo(void) {
    printf("TEST: Verifies that the queue follows the FIFO principle correctly even when working with lots of elements\n");

    Queue q = queue_create();
    void* front = NULL;
    int first = 0;

    queue_enqueue(q, &first);
    for (int i = 1 ; i < BULK_AMOUNT ; i++) {
        queue_enqueue(q, &i);
        front = queue_front(q);
        print_test(*(int*)front == first, "Enqueuing more elements will not change the front of the queue");
    }

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        front = queue_dequeue(q);
        print_test(*(int*)front == i, "The elements are dequeued in the same order as they were added");
        free(front);
    }
    print_test(queue_is_empty(q), "After dequeuing all the elements, the queue is empty");

    queue_destroy(q, NULL);
}

static void test_change_enqueued_value(void) {
    printf("TEST: Verifies that if the value of a variable that was enqueued into a queue is changed, the value inside the queue doesn't change\n");

    Queue q = queue_create();
    void* front = NULL;
    char elem = 'a';

    queue_enqueue(q, &elem);
    front = queue_front(q);
    print_test(*(char*)front == elem, "The element at the front is the one enqueued");

    elem = 'b';
    front = queue_front(q);
    print_test(*(char*)front != elem, "The value of the element at the front must not have changed after changing the value of the element enqueued");

    queue_destroy(q, NULL);
}

void test_emptied_queue(void) {
    printf("TEST: Verifies that an emptied queue acts like a newly created queue\n");

    Queue q = queue_create();
    void* front = NULL;
    int first = 0;
    bool ok = true;

    queue_enqueue(q, &first);
    for (int i = 1 ; i < AMOUNT && q ; i++) {
        queue_enqueue(q, &i);
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

    queue_destroy(q, NULL);
}

int main(void) {
    test_new_queue();
    test_queue_one_element();
    test_fifo();
    test_bulk_fifo();
    test_change_enqueued_value();
    test_emptied_queue();

    return 0;
}

void print_test(bool success, const char* msg) {
    char result[10 + (int)strlen(msg)];
    sprintf(result, "FAIL: %s\n", msg);
    assert_msg(success, result);
}