#include "adt/list.h"
#include "testaux.h"

bool sum_even(const void *elem, const void *extra) {
    int num = *(int *) elem;
    int *sum = (int *) extra;

    if (num % 2 != 0) return false;
    *sum += num;

    return true;
}

void test_new_list() {
    test_start("Create a new List");
    
    list_t *list = list_create(NULL);

    test_true(list_length(list) == 0, "New list should be empty");

    list_get_first(list);
    test_errno_with(ENODATA, "You can't get the first element of an empty List");

    list_get_last(list);
    test_errno_with(ENODATA, "You can't get the last element of an empty List");

    list_remove_first(list);
    test_errno_with(ENODATA, "You can't remove an element from an empty List");

    list_destroy(list);
    test_end();
}

void test_one_element() {
    test_start("Insert one element, get its value, then remove it");
    
    list_t *list = list_create(NULL);
    int num = 4574452;
    void *ptr;
    
    list_insert_first(list, &num);

    test_false(list_length(list) == 0, "The List is no longer empty after inserting an element");
    test_true(list_length(list) == 1, "The List must have only one element after inserting an element");

    ptr = list_get_first(list);
    test_equals(ptr, &num, intcmp, "First element should be the inserted one");
    ptr = NULL;

    test_true(list_length(list) == 1, "The length of the List must not change after getting its first element");

    ptr = list_get_last(list);
    test_equals(ptr, &num, intcmp, "Last element should be the inserted one");
    ptr = NULL;

    test_true(list_length(list) == 1, "The length of the List must not change after getting its last element");
    
    ptr = list_remove_first(list);
    test_equals(ptr, &num, intcmp, "The removed element was the only inserted one");

    test_true(list_length(list) == 0, "The list must be empty after removing its only element");

    list_destroy(list);
    test_end();
}

void test_both_inserts() {
    test_start("For one empty list insert an element at the first position, for \
another empty list insert the same element at the last position and check \
that both lists are equal.");
    
    list_t *list_f = list_create(NULL);
    list_t *list_l = list_create(NULL);
    int num = 10;
    void *ptr_f = NULL, *ptr_l = NULL;

    size_t len_f = list_length(list_f), len_l = list_length(list_l);
    test_true(len_f == 0, "The List must be empty");
    test_equals(&len_f, &len_l, sizecmp, "Both Lists must be empty when created");

    list_insert_first(list_f, &num);
    list_insert_last(list_l, &num);

    len_f = list_length(list_f), len_l = list_length(list_l);
    test_true(len_l == 1, "The List must have one element");
    test_equals(&len_f, &len_l, sizecmp, "Both Lists must have one element");

    ptr_f = list_get_first(list_f), ptr_l = list_get_first(list_l);
    test_not_null(ptr_f, "The element is not null");
    test_not_null(ptr_l, "The element is not null");
    test_equals(ptr_f, ptr_l, intcmp, "Both Lists have the same first value");

    ptr_f = list_get_last(list_f), ptr_l = list_get_last(list_l);
    test_not_null(ptr_f, "The element is not null");
    test_not_null(ptr_l, "The element is not null");
    test_equals(ptr_f, ptr_l, intcmp, "Both Lists have the same last value");

    ptr_f = list_remove_first(list_f), ptr_l = list_remove_first(list_l);
    test_not_null(ptr_f, "The element is not null");
    test_not_null(ptr_l, "The element is not null");
    test_equals(ptr_f, ptr_l, intcmp, "Both Lists returned the same element when removing");

    len_f = list_length(list_f), len_l = list_length(list_l);
    test_true(len_f == 0, "The List must be empty");
    test_equals(&len_f, &len_l, sizecmp, "Both Lists must be empty after removing their elements");

    list_destroy(list_f);
    list_destroy(list_l);
    test_end();
}

void test_insert_start() {
    test_start("Inserting elements at the start of the List won't affect the last element");
    
    list_t *list = list_create(free);
    int *end_num = (int *) malloc(sizeof(int));
    *end_num = MID_AMOUNT * 2;
    void *ptr = NULL;

    list_insert_last(list, end_num);
    
    ptr = list_get_last(list);
    test_equals(ptr, end_num, intcmp, "The element inserted is at the last position");

    for (int i = 0 ; i < MID_AMOUNT ; i++) {
        int *val = (int *) malloc(sizeof(int));
        if (!val) { list_destroy(list) ; test_end() ; return; }
        *val = i;
        list_insert_first(list, val);
        test_not_equals(list_get_last(list), &i, intcmp, "The elements that are inserted at the start of the List won't affect the element at the end of the List");
    }

    test_equals(list_get_last(list), end_num, intcmp, "The last element of the List didn't change after all the inserts");

    list_destroy(list);
    
    test_end();
}

void test_insert_end() {
    test_start("Inserting elements at the end of the List won't affect the first element");
    
    list_t *list = list_create(free);
    int *start_num = (int *) malloc(sizeof(int));
    *start_num = MID_AMOUNT * 2;
    void *ptr = NULL;

    list_insert_first(list, start_num);
    
    ptr = list_get_first(list);
    test_equals(ptr, start_num, intcmp, "The element inserted is at the first position");

    for (int i = 0 ; i < MID_AMOUNT ; i++) {
        int *val = (int *) malloc(sizeof(int));
        if (!val) { list_destroy(list) ; test_end() ; return; }
        *val = i;
        list_insert_last(list, val);
        test_not_equals(list_get_first(list), &i, intcmp, "The elements that are inserted at the end of the List won't affect the element at the start of the List");
    }

    test_equals(list_get_first(list), start_num, intcmp, "The first element of the List didn't change after all the inserts");

    list_destroy(list);
    
    test_end();
}

void test_empty_list() {
    test_start("Insert some elements, then empty the List, check that it works like an empty List");
    
    list_t *list = list_create(NULL);
    void *ptr = NULL;

    for (int i = 0 ; i < MID_AMOUNT ; i++) {
        int *val = (int *) malloc(sizeof(int));
        if (!val) { list_destroy(list) ; test_end() ; return; }
        *val = i;
        list_insert_first(list, val);
    }

    while (list_length(list) > 0) {
        ptr = list_remove_first(list);
        free(ptr);
    }
    
    test_true(list_length(list) == 0, "Empty List should be empty");

    list_get_first(list);
    test_errno_with(ENODATA, "You can't get the first element of an empty List");

    list_get_last(list);
    test_errno_with(ENODATA, "You can't get the last element of an empty List");

    list_remove_first(list);
    test_errno_with(ENODATA, "You can't remove an element from an empty List");

    list_destroy(list);
    test_end();
}

void test_bulk() {
    test_start("The List must work with a lot of elements");
    
    list_t *list = list_create(free);
    void *ptr = NULL;

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        int *val = (int *) malloc(sizeof(int));
        if (!val) { list_destroy(list) ; test_end() ; return ; }
        *val = i;
        list_insert_last(list, val);
        ptr = list_get_last(list);
        test_not_null(ptr, "Get the last element of the list is not NULL");
        test_equals(ptr, &i, intcmp, "The element just inserted must be the new last element of the List");
    }
    
    test_true(list_length(list) == BULK_AMOUNT, "The length is equal of the amount of elements inserted");
    
    for (int i = 0; i < BULK_AMOUNT; i++) {
        int *removed = list_remove_first(list);
        test_equals(removed, &i, intcmp, "Removed element should match insertion order");
        free(removed);
    }
    
    test_true(list_length(list) == 0, "List should be empty after bulk removal");

    list_destroy(list);
    test_end();
}

void test_bulk_destroy() {
    test_start("The List frees the memory of its elements correctly with its destroy operation");
    
    list_t *list = list_create(free);
    void *ptr = NULL;

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        int *val = (int *) malloc(sizeof(int));
        if (!val) { list_destroy(list) ; test_end() ; return ; }
        *val = i;
        list_insert_last(list, val);
        ptr = list_get_last(list);
        test_not_null(ptr, "Get the last element of the list is not NULL");
        test_equals(ptr, &i, intcmp, "The element just inserted must be the new last element of the List");
    }
    
    test_true(list_length(list) == BULK_AMOUNT, "The length is equal of the amount of elements inserted");
    
    list_destroy(list);
    test_end();
}

void test_other_types() {
    test_start("Check that the List can store different datatypes");
    
    list_t *string_list = list_create(NULL);
    list_t *float_list = list_create(NULL);
    void *ptr;

    char *strings[] = {"hello world", "my name is", "goodbye world"};
    float floats[] = {3.14f, 2.5f, 6431.123f};

    for (int i = 0 ; i < 3 ; i++) {
        list_insert_last(string_list, strings[i]);
    }
    for (int i = 0 ; i < 3 ; i++) {
        test_false(list_length(string_list) == 0, "Until there are no elements left in the List, it must not be empty");
        ptr = list_remove_first(string_list);
        test_not_null(ptr, "There must be elements left to be removed");
        test_equals(ptr, strings[i], vstrcmp, "The elements are removed in the same order as they were inserted");
    }

    for (int i = 0 ; i < 3 ; i++) {
        list_insert_last(float_list, &floats[i]);
    }
    for (int i = 0 ; i < 3 ; i++) {
        test_false(list_length(float_list) == 0, "Until there are no elements left in the List, it must not be empty");
        ptr = list_remove_first(float_list);
        test_not_null(ptr, "There must be elements left to be removed");
        test_equals(ptr, &floats[i], floatcmp, "The elements are removed in the same order as they were inserted");
    }

    list_destroy(string_list);
    list_destroy(float_list);
    test_end();
}

void test_internal_iterator() {
    test_start("The internal iterator for the List works correctly");
    
    list_t *list = list_create(NULL);
    int nums[] = {4, 2, 8, 6, 2, 7, 9};
    int sum_ev = 0, sum_all = 0, sum_check = 0;

    for (int i = 0 ; i < 7 ; i++) {
        sum_all += nums[i];
        list_insert_last(list, &nums[i]);
    }
    int i = 0;
    while (nums[i] % 2 == 0 && i < 7) {
        sum_ev += nums[i];
        i++;
    }

    list_for_each(list, sum_even, &sum_check);
    
    test_equals(&sum_check, &sum_ev, intcmp, "The internal iterator sums the correct amount of elements, and cuts at the right place");

    list_destroy(list);
    test_end();
}

void test_new_iter() {
    test_start("Create an iterator for a List");
    
    list_t *list = list_create(NULL);
    list_iterator_t *iter = list_iter_create(list);

    test_false(list_iter_has_next(iter), "An iterator for an empty list should not have any elements to iterate through");
    
    list_iter_next(iter);
    test_errno_with(ENODATA, "Trying to step over to the next element for the iterator with no elements should set errno to ENODATA");

    list_iter_get_current(iter);
    test_errno_with(ENODATA, "Trying to get the current element for the iterator with no elements should set errno to ENODATA");

    list_iter_remove(iter);
    test_errno_with(ENODATA, "Trying to remove the current element for the iterator with no elements should set errno to ENODATA");

    list_iter_destroy(iter);
    list_destroy(list);
    
    test_end();
}

void test_iterate_list() {
    test_start("The external iterator for the List works correctly");
    
    list_t *list = list_create(free);
    int *ptr = NULL, last = -1;

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        int *val = (int *) malloc(sizeof(int));
        if (!val) { list_destroy(list) ; test_end() ; return ; }
        *val = i;
        list_insert_last(list, val);
    }

    list_iterator_t *iter = list_iter_create(list);
    test_true(list_iter_has_next(iter), "List with elements should have next");
    ptr = list_iter_get_current(iter);
    int i = 0;
    test_equals(ptr, &i, intcmp, "The first element of the iterator is the one expected");
    list_iter_next(iter);
    i++;
    last = *ptr;
    
    for ( ; i < BULK_AMOUNT ; i++) {
        test_true(list_iter_has_next(iter), "List must still have elements to iterate through");
        ptr = list_iter_get_current(iter);
        test_greater_than(ptr, &last, intcmp, "The elements are iterated in the expected order");
        last = *ptr;
        list_iter_next(iter);
    }
    
    test_false(list_iter_has_next(iter), "Iterator should have finished");
    
    list_iter_destroy(iter);
    list_destroy(list);

    test_end();
}

void test_iter_insert() {
    test_start("Check that the insert operation of the iterator works correctly");
    
    list_t *list = list_create(NULL);
    int nums[] = {1, 2, 3, 4, 6, 7};
    int new_elem = 5;
    int *ptr = NULL, last = nums[0] - 1;

    for (int i = 0 ; i < 6 ; i++) {
        list_insert_last(list, &nums[i]);
    }

    size_t len = list_length(list);

    list_iterator_t *iter = list_iter_create(list);
    for ( ; list_iter_has_next(iter) ; list_iter_next(iter)) {
        ptr = list_iter_get_current(iter);
        if (*ptr == 6) {
            list_iter_insert(iter, &new_elem);
            break;
        }
    }
    list_iter_destroy(iter);

    bool ok = false;
    
    for (iter = list_iter_create(list) ; list_iter_has_next(iter) ; list_iter_next(iter)) {
        ptr = list_iter_get_current(iter);
        test_true(*ptr == last + 1, "The element was added correctly to the List");
        if (*ptr == 5 && last == 4) ok = true;
        last = *ptr;
    }
    test_true(ok, "The element is in the expected position");
    test_true(list_length(list) == len + 1, "The new length of the List is updated correctly");
    
    list_iter_destroy(iter);
    list_destroy(list);

    test_end();
}

void test_iter_remove() {
    test_start("Check that the remove operation of the iterator works correctly");
    
    list_t *list = list_create(NULL);
    int nums[] = {1, 2, 3, 4, 5, 6, 7};
    int *rem = NULL;
    int *ptr = NULL, last = nums[0];

    for (int i = 0 ; i < 7 ; i++) {
        list_insert_last(list, &nums[i]);
    }

    size_t len = list_length(list);
    
    list_iterator_t *iter = list_iter_create(list);
    for ( ; list_iter_has_next(iter) ; list_iter_next(iter)) {
        ptr = list_iter_get_current(iter);
        if (*ptr == 5) {
            rem = list_iter_remove(iter);
            break;
        }
    }
    test_true(*rem == 5, "The deleted element was the correct one");

    list_iter_destroy(iter);
    bool ok = false;
    for (iter = list_iter_create(list) ; list_iter_has_next(iter) ; list_iter_next(iter)) {
        ptr = list_iter_get_current(iter);
        if (*ptr == 6 && last == 4) ok = true;
        last = *ptr;
    }
    test_true(ok, "The element was removed correctly");
    test_true(list_length(list) == len - 1, "The length of the list is updated correctly");

    list_iter_destroy(iter);
    list_destroy(list);

    test_end();
}

int main(void) {
    test_new_list();
    test_one_element();
    test_both_inserts();
    test_insert_start();
    test_insert_end();
    test_empty_list();
    test_bulk();
    test_bulk_destroy();
    test_other_types();
    test_internal_iterator();

    test_new_iter();
    test_iterate_list();
    test_iter_insert();
    test_iter_remove();

    return 0;
}