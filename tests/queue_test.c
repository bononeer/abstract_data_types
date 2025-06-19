#include "adt/queue.h"
#include "testaux.h"

void test_new_queue(void) {
    test_start("Create a new Queue");

    queue_t *queue = queue_create(NULL);

    test_true(queue_is_empty(queue), "A newly created Queue must be empty");

    queue_front(queue);
    test_errno_with(ENODATA, "You can't get the front of an empty Queue");

    queue_dequeue(queue);
    test_errno_with(ENODATA, "You can't dequeue an element from an empty Queue");

    queue_destroy(queue);

    test_end();
}

void test_one_element(void) {
    test_start("Enqueue an element to the Queue, look at the front, and then dequeue it");

    queue_t *queue = queue_create(NULL);
    int num = 13624;
    void *ptr = NULL;

    queue_enqueue(queue, &num);

    test_false(queue_is_empty(queue), "The Queue is not empty after storing an element");

    ptr = queue_front(queue);
    test_equals(ptr, &num, intcmp, "The element at the front of the Queue must be its only element");
    ptr = NULL;
    
    test_false(queue_is_empty(queue), "The Queue must have the element after only getting the first element");

    ptr = queue_dequeue(queue);
    test_equals(ptr, &num, intcmp, "The dequeued element was the only pushed one");

    test_true(queue_is_empty(queue), "The Queue has to be empty after dequeueing the only element that was enqueued");

    queue_destroy(queue);

    test_end();
}

void test_fifo_property(void) {
    test_start("Check that the Queue structure follows the FIFO Property correctly");

    queue_t *queue = queue_create(NULL);
    int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    void *ptr;

    for (int i = 0 ; i < 10 ; i++) {
        queue_enqueue(queue, &nums[i]);
    }

    for (int i = 0 ; i < 10 ; i++) {
        test_false(queue_is_empty(queue), "Until there are no elements left in the Queue, it must not be empty");
        ptr = queue_dequeue(queue);
        test_not_null(ptr, "There must be elements left to be dequeued");
        test_equals(ptr, &nums[i], intcmp, "The elements are dequeued in the same order as they were enqueued");
    }

    queue_destroy(queue);

    test_end();
}

void test_empty_queue(void) {
    test_start("Enqueue elements into the Queue, dequeue them all, then check that the Queue is the same as a newly created one");

    queue_t *queue = queue_create(NULL);
    int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for (int i = 0 ; i < 10 ; i++) {
        queue_enqueue(queue, &nums[i]);
    }

    for (int i = 0 ; i < 10 ; i++) {
        queue_dequeue(queue);
    }

    test_true(queue_is_empty(queue), "After dequeueing everything, the Queue has to be empty");

    queue_front(queue);
    test_errno_with(ENODATA, "You can't get the front of an empty Queue");

    queue_dequeue(queue);
    test_errno_with(ENODATA, "You can't dequeue an element from an empty Queue");

    queue_destroy(queue);

    test_end();
}

void test_bulk(void) {
    test_start("The Queue must work with a lot of elements");

    queue_t *queue = queue_create(free);
    void *ptr = NULL;

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        int *val = (int *) malloc(sizeof(int));
        if (!val) { queue_destroy(queue); test_end(); return; }

        *val = i;
        queue_enqueue(queue, val);
        ptr = queue_front(queue);
        test_not_null(ptr, "There must be elements left to be dequeued");
        if (i == 0) continue;
        test_not_equals(ptr, &i, intcmp, "The element at the front must not change if other elements are enqueued");
    }

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        ptr = queue_dequeue(queue);
        test_equals(ptr, &i, intcmp, "The element just dequeued is the correct one");
        free(ptr);
    }

    test_true(queue_is_empty(queue), "The Queue is empty once again after dequeueing a lot of elements");

    queue_destroy(queue);

    test_end();
}

void test_bulk_destroy(void) {
    test_start("The Queue frees the memory of its elements correctly with its destroy operation");

    queue_t *queue = queue_create(free);
    void *ptr = NULL;

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        int *val = (int *) malloc(sizeof(int));
        if (!val) { queue_destroy(queue); test_end(); return; }

        *val = i;
        queue_enqueue(queue, val);
        ptr = queue_front(queue);
        test_not_null(ptr, "There must be elements left to be dequeued");
        if (i == 0) continue;
        test_not_equals(ptr, &i, intcmp, "The element at the front must not change if other elements are enqueued");
    }

    test_false(queue_is_empty(queue), "The Queue is not empty before the call to the destroy operation");

    queue_destroy(queue);

    test_end();
}

void test_other_types(void) {
    test_start("Check that the Queue can store different datatypes");

    queue_t *string_queue = queue_create(NULL);
    queue_t *float_queue = queue_create(NULL);
    void *ptr;

    char *strings[] = {"hello world", "my name is", "goodbye world"};
    float floats[] = {3.14f, 2.5f, 6431.123f};

    for (int i = 0 ; i < 3 ; i++) {
        queue_enqueue(string_queue, strings[i]);
    }
    for (int i = 0 ; i < 3 ; i++) {
        test_false(queue_is_empty(string_queue), "Until there are no elements left in the Queue, it must not be empty");
        ptr = queue_dequeue(string_queue);
        test_not_null(ptr, "There must be elements left to be dequeued");
        test_equals(ptr, strings[i], vstrcmp, "The elements are dequeued in the same order as they were enqueued");
    }

    for (int i = 0 ; i < 3 ; i++) {
        queue_enqueue(float_queue, &floats[i]);
    }
    for (int i = 0 ; i < 3 ; i++) {
        test_false(queue_is_empty(float_queue), "Until there are no elements left in the Queue, it must not be empty");
        ptr = queue_dequeue(float_queue);
        test_not_null(ptr, "There must be elements left to be dequeued");
        test_equals(ptr, &floats[i], floatcmp, "The elements are dequeued in the same order as they were enqueued");
    }

    queue_destroy(string_queue);
    queue_destroy(float_queue);

    test_end();
}

int main(void) {
    test_new_queue();
    test_one_element();
    test_fifo_property();
    test_empty_queue();
    test_bulk();
    test_bulk_destroy();
    test_other_types();

    return 0;
}