#include <stdio.h>
#include "adt/queue.h"
#include "adt/stack.h"

/* ############################### Algorithms ############################### */

// Filter the elements of the queue with a filter function. Only the elements 
// that evaluates as true are kept in the queue.
void filter_queue(queue_t *queue, smpl_visit_func_t filter_func, void *extra) {
    queue_t *aux = queue_create(NULL);

    while (!queue_is_empty(queue)) {
        void *front = queue_dequeue(queue);
        if (!front) return;

        if (filter_func(front, extra)) {
            queue_enqueue(aux, front);
        }
    }

    while (!queue_is_empty(aux)) {
        queue_enqueue(queue, queue_dequeue(aux));
    }

    queue_destroy(aux);
}

// Reverse how the elements are currently stored in the Stack with the help of
// a Queue.
void reverse_stack(stack_t *stack) {
    queue_t *aux = queue_create(NULL);

    while (!stack_is_empty(stack)) {
        queue_enqueue(aux, stack_pop(stack));
    }

    while (!queue_is_empty(aux)) {
        stack_push(stack, queue_dequeue(aux));
    }

    queue_destroy(aux);
}

/* ############################## Aux Function ############################## */

bool is_even(const void *elem, const void *extra) {
    int num = *(int *) elem;

    return num % 2 == 0;
}

bool less_chars_than(const void *elem, const void *extra) {
    char *string = (char *) elem;
    size_t len = *(size_t *) extra;

    return strlen(string) < len;
}

/* ################################## Main ################################## */

int main(void) {
    errno = 0;
    // Queues that won't free the memory of its elements.
    queue_t *queue1 = queue_create(NULL);
    queue_t *queue2 = queue_create(NULL);

    int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    char *strings[] = {"hello world", "short", "this one is a very long string", "cute", "cool", "amazing"};
    size_t len = 6;

    for (int i = 0 ; i < 10 ; i++) {
        queue_enqueue(queue1, &nums[i]);
    }

    for (int i = 0 ; i < 6 ; i++) {
        queue_enqueue(queue2, strings[i]);
    }

    filter_queue(queue1, is_even, NULL);
    filter_queue(queue2, less_chars_than, &len);

    printf("The following are all even numbers: \n");
    while (!queue_is_empty(queue1)) {
        printf("%d, ", *(int *) queue_dequeue(queue1));
    }
    printf("\n");

    printf("The following are all strings with less than %ld characters: \n", len);
    while (!queue_is_empty(queue2)) {
        printf("%s, ", (char *) queue_dequeue(queue2));
    }
    printf("\n");

    queue_destroy(queue1);
    queue_destroy(queue2);

    stack_t *stack = stack_create(NULL);

    for (int i = 0 ; i < 6 ; i++) {
        stack_push(stack, strings[i]);
    }

    reverse_stack(stack);

    printf("After reversing the elements, from top to bottom the elements are: \n");
    while (!stack_is_empty(stack)) {
        printf("%s, ", (char *) stack_pop(stack));
    }
    printf("\n");
    
    stack_destroy(stack);

    if (errno != 0) return -errno;
    return 0;
}
