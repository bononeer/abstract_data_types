#include <stdio.h>
#include <string.h>
#include "adt/list.h"

/* ############################### Algorithms ############################### */

// Map the elements of the list with a map function. Apply the function to every 
// element of the list.
void map_list(list_t *list, void (*map_func)(void *, void *), void *extra) {
    list_iterator_t *iter = list_iter_create(list);

    while (list_iter_has_next(iter)) {
        void *cur = list_iter_get_current(iter);
        map_func(cur, extra);
        list_iter_next(iter);
    }

    list_iter_destroy(iter);
}

list_t *merge(list_t *, list_t *);
list_t *mergesort(list_t *);

// Sort a floating point number List (with a mergesort algorithm).
list_t *list_sort(list_t *list) {
    if (!list) return NULL;

    list_t *copy = list_create(NULL);
    if (!copy) return NULL;

    list_iterator_t *iter = list_iter_create(list);
    if (!iter) {
        list_destroy(copy);
        return NULL;
    }

    while (list_iter_has_next(iter)) {
        list_insert_last(copy, list_iter_get_current(iter));
        list_iter_next(iter);
    }

    list_iter_destroy(iter);

    list_t *sorted = mergesort(copy);
    if (!sorted) {
        list_destroy(copy);
        return NULL;
    }

    return sorted;
}

/* ############################## Aux Function ############################## */

void square(void *ptr1, void *null) {
    (void) null;
    int num = *(int *) ptr1;
    int *res = ptr1;
    *res = num * num;
}

list_t *merge(list_t *left, list_t *right) {
    list_t *merged = list_create(NULL);
    if (!merged) return NULL;

    list_iterator_t *left_iter = list_iter_create(left);
    list_iterator_t *right_iter = list_iter_create(right);
    if (!left_iter || !right_iter) {
        if (left_iter) list_iter_destroy(left_iter);
        if (right_iter) list_iter_destroy(right_iter);
        list_destroy(merged);
        return NULL;
    }
    
    while (list_iter_has_next(left_iter) && list_iter_has_next(right_iter)) {
        float *left_num = list_iter_get_current(left_iter);
        float *right_num = list_iter_get_current(right_iter);
        if (*left_num <= *right_num) {
            list_insert_last(merged, left_num);
            list_iter_next(left_iter);
        } else {
            list_insert_last(merged, right_num);
            list_iter_next(right_iter);
        }
    }

    while (list_iter_has_next(left_iter)) {
        list_insert_last(merged, list_iter_get_current(left_iter));
        list_iter_next(left_iter);
    }

    while (list_iter_has_next(right_iter)) {
        list_insert_last(merged, list_iter_get_current(right_iter));
        list_iter_next(right_iter);
    }

    list_iter_destroy(left_iter);
    list_iter_destroy(right_iter);
    
    list_destroy(left);
    list_destroy(right);

    return merged;
}

list_t *mergesort(list_t *list) {
    if (!list) return NULL;

    size_t len = list_length(list);
    if (len <= 1) {
        return list;
    }

    size_t mid = len / 2;
    list_t *left = list_create(NULL);
    list_t *right = list_create(NULL);
    if (!left || !right) {
        if (left) list_destroy(left);
        if (right) list_destroy(right);
        return list;
    }

    for (size_t i = 0 ; i < mid ; i++) list_insert_last(left, list_remove_first(list));
    while (list_length(list) > 0) list_insert_last(right, list_remove_first(list));

    list_destroy(list);

    left = mergesort(left);
    right = mergesort(right);

    return merge(left, right);
}

/* ################################## Main ################################## */

int main(void) {
    // Queues that won't free the memory of its elements.
    list_t *list = list_create(NULL);

    int nums[] = {1, 2, 3, 4, 5};

    for (int i = 0 ; i < 5 ; i++) {
        list_insert_last(list, &nums[i]);
    }
    
    map_list(list, square, NULL);

    list_iterator_t *iter;

    printf("Elements after applying map function:\n");
    for (iter = list_iter_create(list) ; list_iter_has_next(iter) ; list_iter_next(iter)) {
        printf("%d, ", *(int *) list_iter_get_current(iter));
    }
    list_iter_destroy(iter);
    printf("\n");

    list_destroy(list);

    list = list_create(NULL);
    float fnums[] = {2.5f, 1.63f, 31.12f, 2.76f, 9.31f, 33.3333f, 55.2f, 5.3f};

    for (int i = 0 ; i < 8 ; i++) {
        list_insert_last(list, &fnums[i]);
    }

    list_t *sorted_list = list_sort(list);

    printf("Sorted elements: \n");
    for (iter = list_iter_create(sorted_list) ; list_iter_has_next(iter) ; list_iter_next(iter)) {
        void *ptr = list_iter_get_current(iter);
        printf("%f, ", *(float *) ptr);
    }
    list_iter_destroy(iter);
    printf("\n");

    list_destroy(list);
    list_destroy(sorted_list);

    return 0;
}
