#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../priority_queue/priority_queue.h"
#include "assert_msg.h"

typedef struct {
    char* string;
    int integer;
} MyStruct;

static void print_test(bool, const char*);
static int intcmp(void *num1, void *num2);
static int floatcmp(void *num1, void *num2);
static int minfloatcmp(void *num1, void *num2);
static int structcmp(void *s1, void *s2);
static int sortcmp(const void *a, const void *b);
static MyStruct* struct_create(char* string, int integer);
static void struct_destroy(void* value);

static void test_new_priority_queue(void) {
    printf("TEST: A newly created priority queue works as expected.\n");

    PriorityQueue q = p_queue_create(intcmp, NULL);

    print_test(q != NULL, "Create a new priority queue");
    print_test(p_queue_is_empty(q), "A newly created priority queue must be empty");
    print_test(p_queue_size(q) == 0, "A newly created priority queue must not have any element enqueued");

    print_test(p_queue_dequeue(q) == NULL, "An empty priority queue returns NULL if its tries to dequeue");
    print_test(p_queue_max(q) == NULL, "An empty priority queue returns NULL if its tries to peek at the element with maximum priority");

    p_queue_destroy(q);

    int num = 0;
    void *elements[] = {&num};
    q = p_queue_create_array(elements, 0, intcmp, NULL);

    print_test(q != NULL, "Create a new priority queue from an empty array");
    print_test(p_queue_is_empty(q), "A newly created priority queue from an empty array must be empty");
    print_test(p_queue_size(q) == 0, "A newly created priority queue from an empty array must not have any element enqueued");

    print_test(p_queue_dequeue(q) == NULL, "An empty priority queue from an empty array returns NULL if its tries to dequeue");
    print_test(p_queue_max(q) == NULL, "An empty priority queue from an empty array returns NULL if its tries to peek at the element with maximum priority");

    p_queue_destroy(q);
}

static void test_destroy(void) {    // Check with valgrind
    printf("TEST: Check that the memeory of the elements in the priority queue are freed correctly when using the destroy operation\n");

    PriorityQueue q1 = p_queue_create(intcmp, NULL);
    int num = 2;

    print_test(p_queue_enqueue(q1, &num), "The element was enqueued correctly"); // No allocs for element's memory

    p_queue_destroy(q1);

    PriorityQueue q2 = p_queue_create(intcmp, free);
    int* another_num = (int*)malloc(sizeof(int));
    if (another_num == NULL) { p_queue_destroy(q2); print_test(another_num != NULL, ""); }
    *another_num = 3;

    print_test(p_queue_enqueue(q2, another_num), "The element was enqueued correctly"); // Element with allocated memory
    
    p_queue_destroy(q2);
}

void test_enqueue(void) {
    printf("TEST: Check that the enqueue operation works correctly with only one element\n");

    PriorityQueue q = p_queue_create(intcmp, NULL);
    int num = 3;
    void *ptr = NULL;

    print_test(p_queue_size(q) == 0, "The priority queue size must be zero before enqueueing the element");
    print_test(p_queue_enqueue(q, &num), "The element is enqueued correctly");
    print_test(p_queue_size(q) == 1, "The priority queue size must be one after enqueueing the element");

    ptr = p_queue_max(q);
    print_test(*(int*)ptr == num, "The element with the highest priority is the only one enqueued");

    p_queue_destroy(q);
}

void test_dequeue(void) {
    printf("TEST: Check that the dequeue operation works correctly with only one element\n");

    int num1 = 3, num2 = 4;
    void *arr[] = {&num1, &num2};
    PriorityQueue q = p_queue_create_array(arr, 2, intcmp, NULL);
    void *ptr = NULL;

    print_test(!p_queue_is_empty(q), "The priority queue cannot be empty while dequeueing the element");
    print_test(p_queue_size(q) == 2, "There must be 2 elements after creating the priority queue from an array");

    ptr = p_queue_max(q);
    print_test(*(int*)ptr == *(int*)arr[1], "The element with maximum priority is the one expected to be dequeued first");
    ptr = p_queue_dequeue(q);
    print_test(*(int*)ptr == *(int*)arr[1], "The element dequeued is the one that had the maximum priority");

    print_test(p_queue_size(q) == 1, "There must only one element after dequeuing the one with maximum priority");
    ptr = p_queue_max(q);
    print_test(*(int*)ptr == *(int*)arr[0], "The element with maximum priority is the only one left in the priority queue");

    p_queue_destroy(q);
}

void test_priority_queue_one_element(void) {
    printf("TEST: Tries the flow of enqueueing one element, peeking at the max priority one and dequeueing it out of the priority queue.\n");

    PriorityQueue q = p_queue_create(floatcmp, NULL);
    void *ptr = NULL;
    float elem = 10.21f;

    print_test(p_queue_is_empty(q), "The priority queue is empty before adding any element");
    print_test(p_queue_size(q) == 0, "The size of the priority queue before adding any element is 0");

    size_t previous_size = p_queue_size(q);
    p_queue_enqueue(q, &elem);

    print_test(!p_queue_is_empty(q), "After enqueueing the element the priority queue must not be empty");
    print_test(p_queue_size(q) == previous_size + 1, "The size of the priority queue went up by one after enqueueing the element");

    ptr = p_queue_max(q);
    print_test(*(float*)ptr == elem, "The only element inside the priority queue is the one expected");
    print_test(!p_queue_is_empty(q), "Getting the value of the element with maximum priority does not dequeue it");
    print_test(p_queue_size(q) == previous_size + 1, "The size of the priority queue must not change after getting the value of the element with maximum priority");

    ptr = p_queue_max(q);
    print_test(*(float*)ptr == elem, "While the maximum-priority element in the priority queue is not dequeued, the element with maximum priority will always be the same");

    previous_size = p_queue_size(q);
    ptr = p_queue_dequeue(q);
    print_test(*(float*)ptr == elem, "Dequeueing the element returns the one with the maximum priority");
    print_test(p_queue_is_empty(q), "After dequeueing its only element, the priority queue is empty");
    print_test(p_queue_size(q) == previous_size - 1, "The size of the priority queue went down by one after dequeueing its only element");

    p_queue_destroy(q);
}

void test_priority_order(void) {
    printf("TEST: Insert elements in no particular order, then check that the elements are dequeued by their priority\n");

    PriorityQueue q1 = p_queue_create(intcmp, NULL);
    PriorityQueue q2 = p_queue_create(intcmp, NULL);
    int arr[] = {3, 6, 1, 4, 8, 2, 9, 5, 0, 7};
    int size = 10;
    void *ptr = NULL;

    for (int i = 0 ; i < size ; i++) p_queue_enqueue(q1, &arr[i]);
    for (int i = size - 1 ; i >= 0 ; i--) p_queue_enqueue(q2, &arr[i]);
    print_test(p_queue_size(q1) == size, "All the elements were enqueued into the first priority queue");
    print_test(p_queue_size(q2) == size, "All the elements were enqueued into the second priority queue");

    for (int i = size - 1 ; i >= 0 ; i--) {
        print_test(!p_queue_is_empty(q1), "The first priority queue must not be empty before dequeueing an elemenet");
        print_test(!p_queue_is_empty(q2), "The second priority queue must not be empty before dequeueing an elemenet");
        ptr = p_queue_dequeue(q1);
        print_test(*(int*)ptr == i, "The elements are dequeued by their priority");
        ptr = p_queue_dequeue(q2);
        print_test(*(int*)ptr == i, "The elements are dequeued by their priority");
    }

    p_queue_destroy(q1);
    p_queue_destroy(q2);
}

void test_minimum_priority_queue(void) {
    printf("TEST: Inverting the result of the cmp function will make the maximum-priority queue work as a minimum-priority queue\n");

    float nums[] = {3.14f, 0.125f, 2.71f, 8.12f, 0.125f, 77.21f, 122.f, 12.121212f, 2.5f};
    void *arr[9];

    for (int i = 0 ; i < 9 ; i++) arr[i] = &nums[i];

    PriorityQueue q = p_queue_create_array(arr, 9, minfloatcmp, NULL);
    void *ptr = NULL;
    float previous;

    ptr = p_queue_dequeue(q);
    while (!p_queue_is_empty(q)) {
        previous = *(float*)ptr;
        ptr = p_queue_dequeue(q);
        print_test(*(float*)ptr >= previous, "In a minimum-priority queue, the first elements to be dequeued are the ones with the lowest priority");
    }

    p_queue_destroy(q);
}

void test_bulk_charge_priority_queue(void) {
    printf("TEST: Enqueue a huge amount of elements in the priority queue, then dequeue them all until it is empty\n");

    PriorityQueue q = p_queue_create(intcmp, free);
    void *ptr = NULL, *max_priority = NULL;
    int arr[BULK_AMOUNT];

    srand(2050);

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        int *value = (int*)malloc(sizeof(int));
        if (value == NULL) {p_queue_destroy(q); print_test(value != NULL, "");}
        *value = rand();
        if (max_priority == NULL || *(int*)max_priority < *value) max_priority = value;
        p_queue_enqueue(q, value);
        arr[i] = *value;
        ptr = p_queue_max(q);
        print_test(*(int*)ptr == *(int*)max_priority, "The priority of the elements is kept correctly");
    }

    qsort(arr, BULK_AMOUNT, sizeof(int), sortcmp);

    for (int i = BULK_AMOUNT - 1 ; i >= 0 ; i--) {
        ptr = p_queue_dequeue(q);
        print_test(*(int*)ptr == arr[i], "The elements are dequeued in the correct priority order");
        free(ptr);
    }

    print_test(p_queue_is_empty(q), "An emptied priority queue must be empty");
    print_test(p_queue_size(q) == 0, "An emptied priority queue must not have any element enqueued");

    print_test(p_queue_dequeue(q) == NULL, "An emptied priority queue returns NULL if its tries to dequeue");
    print_test(p_queue_max(q) == NULL, "An emptied priority queue returns NULL if its tries to peek at the element with maximum priority");

    p_queue_destroy(q);
}

void test_create_from_array_is_equal_to_enqueue_one_by_one(void) {
    printf("TEST: Verify that enqueueing all the elements one by one in a priority queue is equal to a priority queue created from an array with the same elements\n");

    PriorityQueue q1 = p_queue_create(floatcmp, NULL);
    int nums[AMOUNT];
    int nums_to_sort[AMOUNT];
    void *arr[AMOUNT];
    void *ptr = NULL;

    for (int i = 0 ; i < AMOUNT ; i++) {
        nums[i] = rand();
        p_queue_enqueue(q1, &nums[i]);
        arr[i] = &nums[i];
        nums_to_sort[i] = nums[i];
    }

    PriorityQueue q2 = p_queue_create_array(arr, AMOUNT, intcmp, NULL);

    print_test(p_queue_size(q1) == p_queue_size(q2), "Both priority queues have the same size");
    print_test(p_queue_size(q1) == AMOUNT, "The priority queues have the correct amount of elements");

    qsort(nums_to_sort, AMOUNT, sizeof(int), sortcmp);

    for (int i = AMOUNT - 1 ; i >= 0 ; i--) {
        ptr = p_queue_dequeue(q1);
        print_test(*(int*)ptr == nums_to_sort[i], "The priority from the priority queue where the elements were enqueued one by one is correct");
        ptr = p_queue_dequeue(q2);
        print_test(*(int*)ptr == nums_to_sort[i], "The priority from the priority queue created from the array is correct");
    }

    p_queue_destroy(q1);
    p_queue_destroy(q2);
}

void test_struct_values(void) {
    printf("TEST: Enqueue structs into the priority queue and check that it works correctly\n");

    PriorityQueue q = p_queue_create(structcmp, *struct_destroy);
    char *string1 = "ZZZmorepriority", *string2 = "AAAlesspriority"; 
    int integer1 = 10, integer2 = -14;
    MyStruct *my_struct1 = struct_create(string1, integer1), *my_struct2 = struct_create(string2, integer2);
    MyStruct *ptr = NULL;

    print_test(q != NULL, "The priority queue is created correctly with a specific destroy function");

    print_test(p_queue_is_empty(q), "The priority queue must be empty before enqueueing any struct");
    print_test(p_queue_enqueue(q, my_struct1), "The struct can be enqueued correctly");
    print_test(p_queue_enqueue(q, my_struct2), "The struct can be enqueued correctly");
    print_test(!p_queue_is_empty(q), "The priority queue must not be empty after enqueueing the structs");

    ptr = (MyStruct*)p_queue_dequeue(q);
    print_test(strcmp(ptr->string, string1) == 0, "The struct that was in the first position is the first one added");
    print_test(ptr->integer == integer1, "The struct data was stored correctly");
    struct_destroy(ptr);
    ptr = NULL;

    print_test(!p_queue_is_empty(q), "There must still be a struct left");
    
    ptr = (MyStruct*)p_queue_max(q);
    print_test(!p_queue_is_empty(q), "Peeking at the struct with maximum priority of a priority queue with struct types does not make it empty");
    print_test(strcmp(ptr->string, string2) == 0, "The new maximum priority struct is the last one added");
    print_test(ptr->integer == integer2, "The struct data was stored correctly");

    p_queue_destroy(q);
}

int main(void) {
    test_new_priority_queue();
    test_destroy();
    test_enqueue();
    test_dequeue();
    test_priority_queue_one_element();
    test_priority_order();
    test_minimum_priority_queue();
    test_bulk_charge_priority_queue();
    test_create_from_array_is_equal_to_enqueue_one_by_one();
    test_struct_values();

    return 0;
}

void print_test(bool success, const char* msg) {
    char result[10 + (int)strlen(msg)];
    sprintf(result, "FAIL: %s\n", msg);
    assert_msg(success, result);
}

int intcmp(void *num1, void *num2) {
    return *(int*)num1 - *(int*)num2;
}

int floatcmp(void *num1, void *num2) {
    float float_res = *(float*)num1 - *(float*)num2;
    return float_res == 0 ? 0 : float_res > 0 ? 1 : -1;
}

int minfloatcmp(void *num1, void *num2) {
    return floatcmp(num2, num1);
}

int structcmp(void *s1, void *s2) {
    MyStruct *struct1 = (MyStruct*)s1;
    MyStruct *struct2 = (MyStruct*)s2;
    return strcmp(struct1->string, struct2->string);
}

int sortcmp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
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