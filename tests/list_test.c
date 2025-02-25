#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../list/list.h"
#include "assert_msg.h"

typedef struct {
    char* string;
    int integer;
} MyStruct;

static void print_test(bool, const char*);
static MyStruct* struct_create(char* string, int integer);
static void struct_destroy(void* value);
static bool sum_all(void *elem, void *extra);
static bool cut_condition(void* elem, void* extra);
static bool copy_list(void* elem, void* extra);

void test_new_list(void) {
    printf("TEST: A newly created list works as expected\n");

    List l = list_create(NULL);

    print_test(l != NULL, "Create a new list");
    print_test(list_is_empty(l), "The list must be empty");
    print_test(list_length(l) == 0, "The length of an empty list must be equal to 0");
    print_test(list_delete_first(l) == NULL, "Trying to delete the first element of an empty list returns NULL");
    print_test(list_get_first(l) == NULL, "Trying to get the first element of an empty list returns NULL");
    print_test(list_get_last(l) == NULL, "Trying to get the last element of an empty list returns NULL");

    list_destroy(l);
}

static void test_destroy(void) {    // Check with valgrind
    printf("TEST: Check that the memeory of the elements in the stack are freed correctly when using the destroy operation\n");

    List l1 = list_create(NULL);
    int num = 2;

    print_test(list_insert_first(l1, &num), "The element was inserted correctly"); // No allocs for element's memory

    list_destroy(l1);

    List l2 = list_create(free);
    int* another_num = (int*)malloc(sizeof(int));
    print_test(another_num != NULL, "");
    *another_num = 3;

    print_test(list_insert_first(l2, another_num), "The element was inserted correctly"); // Element with allocated memory
    
    list_destroy(l2);
}

void test_list_one_element(void) {
    printf("TEST: Insert an element, get its value, delete it\n");

    List l = list_create(NULL);
    void* ptr = NULL;
    int elem = 2;

    print_test(list_insert_first(l, &elem), "The element is inserted in the first position");
    print_test(!list_is_empty(l), "After inserting an element the list must not be empty");
    print_test(list_length(l) == 1, "The length of the list goes up by one after inserting an element");

    ptr = list_get_first(l);
    print_test(*(int*)ptr == elem, "The element in the first position of the list is the one inserted");
    print_test(!list_is_empty(l), "Getting the element at the first position does not delete it or make the list empty");
    print_test(list_length(l) == 1, "Getting the element at the first position does not change the length of the list");

    ptr = list_get_last(l);
    print_test(*(int*)ptr == elem, "The element in the last position of the list is the one inserted in the first position");
    print_test(!list_is_empty(l), "Getting the element at the last position does not delete it or make the list empty");
    print_test(list_length(l) == 1, "Getting the element at the last position does not change the length of the list");

    ptr = list_delete_first(l);
    print_test(*(int*)ptr == elem, "The element deleted from the list is the one inserted");
    print_test(list_is_empty(l), "After deleting the element the list must become empty");
    print_test(list_length(l) == 0, "The length of the list must become 0 after deleting its only element");

    list_destroy(l);
}

void test_insert_first_is_equal_to_insert_last_for_an_empty_list(void) {
    printf("TEST: After inserting an element to an empty list, verify that the first and last element are the same element\n");

    List l = list_create(NULL);
    void *first = NULL, *last = NULL;
    float elem = 3.14f;

    list_insert_last(l, &elem);
    first = list_get_first(l);
    last = list_get_last(l);

    print_test(*(float*)first == elem, "The first element has the correct value");
    print_test(*(float*)last == elem, "The last element has the correct value");
    print_test(first == last, "The element inserted in the empty list is the first element and the last element at the same time");
    print_test(list_length(l) == 1, "There must be only one element in the list");

    list_destroy(l);
}

void test_delete_one_element_multiple_appearances(void) {
    printf("TEST: Insert an element multiple times, delete the first element from the list, check that only one appearance of it has been deleted\n");

    List l = list_create(NULL);
    void* ptr = NULL;
    char elem = 'a';

    for (int i = 0 ; i < AMOUNT ; i++) {
        list_insert_last(l, &elem);
    }
    print_test(list_length(l) == AMOUNT, "The list has a good amount of elements inserted");

    ptr = list_delete_first(l);
    print_test(*(char*)ptr == 'a', "The element deleted is the same as the one inserted multiple times");
    print_test(list_length(l) == AMOUNT - 1, "Even tough one of the elements was deleted, all the other appearances of it are not deleted");
    print_test(!list_is_empty(l), "The list must not be empty after one of the elements was deleted");

    list_destroy(l);
}

void test_insert_at_the_end(void) {
    printf("TEST: Inserting elements at the end of the list do not affect its first element\n");

    List l = list_create(NULL);
    void *first = NULL, *last = NULL;
    int elem_first = 1, elem = 99;

    list_insert_first(l, &elem_first);
    first = list_get_first(l);
    print_test(*(int*)first == elem_first, "The element was inserted correctly at the first position");

    for (int i = 0 ; i < AMOUNT ; i++) {
        list_insert_last(l, &elem);
        first = list_get_first(l);
        last = list_get_last(l);
        print_test(*(int*)first != *(int*)last, "Inserting elements in the last position do not affect the element at the first position");
    }

    list_destroy(l);
}

void test_insert_at_the_start(void) {
    printf("TEST: Inserting elements at the start of the list do not affect its last element\n");

    List l = list_create(NULL);
    void *first = NULL, *last = NULL;
    int elem_last = 1, elem = 99;

    list_insert_last(l, &elem_last);
    last = list_get_last(l);
    print_test(*(int*)last == elem_last, "The element was inserted correctly at the last position");

    for (int i = 0 ; i < AMOUNT ; i++) {
        list_insert_first(l, &elem);
        first = list_get_first(l);
        last = list_get_last(l);
        print_test(*(int*)first != *(int*)last, "Inserting elements in the first position do not affect the element at the last position");
    }

    list_destroy(l);
}

void test_emptied_list(void) {
    printf("TEST: Verifies that an empty list acts like a newly created list\n");

    List l = list_create(free);
    void *first = NULL, *last = NULL;
    bool ok = true;

    for (int i = 0 ; i < AMOUNT && ok; i++) {
        int* value = (int*)malloc(sizeof(int));
        print_test(value != NULL, "");
        *value = i;
        list_insert_last(l, value);
        last = list_get_last(l);
        ok = *(int*)last == i && !list_is_empty(l);
    }
    print_test(ok, "A good amount of element can be inserted in the list");

    for (int i = 0 ; i < AMOUNT && ok ; i++) {
        first = list_delete_first(l);
        ok = *(int*)first == i;
        free(first);
    }

    print_test(list_is_empty(l), "An emptied list must be empty");
    print_test(list_length(l) == 0, "The length of an empty list must be equal to 0");
    print_test(list_delete_first(l) == NULL, "Trying to delete the first element of an empty list returns NULL");
    print_test(list_get_first(l) == NULL, "Trying to get the first element of an empty list returns NULL");
    print_test(list_get_last(l) == NULL, "Trying to get the last element of an empty list returns NULL");

    list_destroy(l);
}

void test_sum_all_with_internal_iterator(void) {
    printf("TEST: Get the sum of all the elements in the list with a for each iteration\n");

    List l = list_create(NULL);
    size_t length = 5;
    int elements[5] = {1, 2, 3, 4, 5};
    int sum = 0, sum_for_each = 0;

    for (size_t i = 0 ; i < length ; i++) {
        list_insert_first(l, &elements[i]);
        sum += elements[i];
    }
    list_for_each(l, sum_all, &sum_for_each);

    print_test(sum == sum_for_each, "The for each iteration sums all the elements in the list correctly");

    list_destroy(l);
}

void test_cut_condition_with_internal_iterator(void) {
    printf("TEST: Test a for each iteration with a cut condition\n");

    List l = list_create(NULL);
    size_t length = 14;
    int elements[14] = {2, 1, 8, 7, 5, 0, 3, 8, 2, 1, 10, 23, 12, 59};
    int iterations = 0;

    for (size_t i = 0 ; i < length ; i++) {
        list_insert_last(l, &elements[i]);
    }
    print_test(list_length(l) == length, "Every element has been inserted correctly");

    list_for_each(l, cut_condition, &iterations);
    print_test(iterations != length, "The iteration must have stopped before reaching the last element in the list");

    list_destroy(l);
}

void test_bulk_internal_iterator(void) {
    printf("TEST: Iterate through a list with a huge amount elements with a for each iteration with no cut condition\n");

    List l1 = list_create(free);
    List l2 = list_create(free);
    void *ptr1 = NULL, *ptr2 = NULL;
    bool ok = true;

    for (int i = 0 ; i < BULK_AMOUNT && ok; i++) {
        int* value = (int*)malloc(sizeof(int));
        print_test(value != NULL, "");
        *value = i;
        list_insert_last(l1, value);
        ptr1 = list_get_last(l1);
        ok = *(int*)ptr1 == i;
    }
    print_test(ok, "A huge amount of elements can be inserted in a list");

    list_for_each(l1, copy_list, &l2);
    
    print_test(list_length(l1) == list_length(l2), "The for each iteration must have gone through every element of the list");
    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        ptr1 = list_delete_first(l1);
        ptr2 = list_delete_first(l2);
        print_test(*(int*)ptr1 == *(int*)ptr2, "The given visit function works as expected");

        free(ptr1);
    }

    list_destroy(l1);
    list_destroy(l2);
}

void test_iterator_for_empty_list(void) {
    printf("TEST: An iterator created for an empty list must act as a finished iterator\n");

    List l = list_create(NULL);
    ListIterator iter = list_iter_create(l);

    print_test(iter != NULL, "Create a new List Iterator");
    print_test(!list_iter_has_next(iter), "An iterator for an empty list must not have elements to iterate through");
    print_test(!list_iter_next(iter), "The iterator can not advance to the next element as there are none to iterate through inside the list");
    print_test(list_iter_get_current(iter) == NULL, "Trying to get the current element of an iteration with no elements to iterate through returns NULL");
    print_test(list_iter_delete(iter) == NULL, "Trying to delete the current element of an iteration with no elements to iterate through returns NULL");

    list_iter_destroy(iter);
    list_destroy(l);
}

void test_bulk_iterate_trough_a_list(void) {
    printf("TEST: Iterate through a list with a lot of elements and verify the order is kept correctly\n");

    List l = list_create(free);
    void* current = NULL;

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        int* value = (int*)malloc(sizeof(int));
        print_test(value != NULL, "");
        *value = i;
        list_insert_last(l, value);
    }

    ListIterator iter = list_iter_create(l);
    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        print_test(list_iter_has_next(iter), "There must still be elements left to iterate through");
        current = list_iter_get_current(iter);
        print_test(*(int*)current == i, "The current element at the iteration is the expected one");
        list_iter_next(iter);
    }

    list_iter_destroy(iter);
    list_destroy(l);
}

void test_iteration_insert_first_position(void) {
    printf("TEST: Insert an element in the position where the iterator is created is equivalent to use the insert first operation of the list\n");

    List l1 = list_create(NULL);
    List l2 = list_create(NULL);
    void *ptr = NULL, *current = NULL;    
    int elem = 18;

    for (int i = 0 ; i < AMOUNT ; i++) {
        list_insert_last(l1, &i);
        list_insert_last(l2, &i);
    }
    size_t len1 = list_length(l1), len2 = list_length(l2);

    print_test(list_insert_first(l1, &elem), "The element is inserted in the first position");
    print_test(list_length(l1) == len1 + 1, "The length of the list goes up by one after inserting an element");

    ptr = list_get_first(l1);
    print_test(*(int*)ptr == elem, "The element in the first position of the list is the one inserted");

    ListIterator iter = list_iter_create(l2);
    print_test(list_iter_insert(iter, &elem), "The element is inserted where the iterator is created");
    print_test(list_length(l1) == len2 + 1, "The length of the list goes up by one after inserting an element with the iterator");

    current = list_iter_get_current(iter);
    ptr = list_get_first(l2);
    print_test(*(int*)ptr == *(int*)current, "The element in the first position of the list is the one inserted with the iterator");

    list_iter_destroy(iter);
    list_destroy(l1);
    list_destroy(l2);
}

void test_iteration_insert_last_position(void) {
    printf("TEST: Insert an element with the iterator in the position where there is no more elements to iterate through is equivalent to the insert last operation of the list\n");

    List l = list_create(NULL);
    void* ptr = NULL;
    int elem1 = 18, elem2 = 111, counter = 0;
    ListIterator iter;

    list_insert_first(l, &elem1);
    list_insert_last(l, &elem2);
    
    ptr = list_get_last(l);
    print_test(*(int*)ptr == elem2, "The element at the end of the list is the one that was inserted in the last position");
    ptr = list_get_first(l);
    print_test(*(int*)ptr != elem2, "The element at the start of the list is not the one that was inserted in the last position");
    print_test(*(int*)ptr == elem1, "The element at the start of the list is the one that was inserted in the first position");
    print_test(list_length(l) == 2, "The list must only have the two elements inserted");
    
    for (iter = list_iter_create(l) ; counter < list_length(l) - 1 ; list_iter_next(iter)) counter++;
    print_test(list_iter_has_next(iter), "There must still be the last element to iterate through");
    ptr = list_iter_delete(iter);
    print_test(*(int*)ptr == elem2, "The element deleted with the iterator at the last position is the inserted at the last position before");
    print_test(list_length(l) == 1, "The list must only have one element inserted after the deletion");

    print_test(list_iter_insert(iter, &elem2), "The iterator inserts the element at the end of the list");

    ptr = list_get_last(l);
    print_test(*(int*)ptr == elem2, "The element at the end of the list is the one that was inserted with the iterator");
    ptr = list_get_first(l);
    print_test(*(int*)ptr != elem2, "The element at the start of the list is not the one that was inserted with the iterator");
    print_test(*(int*)ptr == elem1, "The element at the start of the list is the one that was inserted in the first position");
    print_test(list_length(l) == 2, "The list must only have the two elements inserted again");

    list_iter_destroy(iter);
    list_destroy(l);
}

void test_iteration_insert_middle(void) {
    printf("TEST: Insert an element at the middle of the list with the iterator\n");

    List l = list_create(NULL);
    ListIterator iter;
    void* ptr = NULL;
    int elem = AMOUNT+1, position = AMOUNT / 2, counter = -1;

    for (int i = 0 ; i < AMOUNT ; i++) list_insert_last(l, &i);

    print_test(list_length(l) == AMOUNT, "All the elements were inserted");
    print_test(!list_is_empty(l), "The list must not be empty after inserting all those elements");

    iter = list_iter_create(l);
    for (int i = 0 ; i < (int)position ; i++) {
        print_test(list_iter_has_next(iter), "The iterator must have elements to iterate through");
        list_iter_next(iter);
    }

    list_iter_insert(iter, &elem);
    list_iter_destroy(iter);

    iter = list_iter_create(l);
    do {
        ptr = list_iter_get_current(iter);
        counter++;
        list_iter_next(iter);
    } while (list_iter_has_next(iter) && *(int*)ptr != elem);

    print_test(*(int*)ptr == elem, "The element is the one inserted in the middle");
    print_test(counter == position, "The element was inserted in the correct position");

    list_iter_destroy(iter);
    list_destroy(l);
}

void test_iteration_delete_first(void) {
    printf("TEST: Delete the first element while iterating changes the first element of the original list to the corresponding element\n");

    List l = list_create(free);
    ListIterator iter;
    void *ptr = NULL, *deleted = NULL;

    for (int i = 0 ; i <= AMOUNT ; i++) {
        int* value = (int*)malloc(sizeof(int));
        print_test(value != NULL, "");
        *value = i;
        list_insert_first(l, value);
    }

    ptr = list_get_first(l);
    print_test(*(int*)ptr == AMOUNT, "The element at the first position is the one expected");

    iter = list_iter_create(l);
    deleted = list_iter_delete(iter);
    print_test(*(int*)ptr == *(int*)deleted, "The element deleted is the one that was at the first position before");
    ptr = list_get_first(l);
    print_test(*(int*)ptr != *(int*)deleted, "The new first element is different from the one deleted");
    print_test(*(int*)ptr == AMOUNT-1, "The element at the first position is the corresponding one");

    free(deleted);
    list_iter_destroy(iter);
    list_destroy(l);
}

void test_struct_values(void) {
    printf("TEST: Insert structs into the list and check that it works correctly\n");

    List l = list_create(*struct_destroy);
    char *string1 = "Hello World", *string2 = "This is a struct!"; 
    int integer1 = 10, integer2 = -14;
    MyStruct *my_struct1 = struct_create(string1, integer1), *my_struct2 = struct_create(string2, integer2);
    MyStruct* ptr = NULL;

    print_test(l != NULL, "The list is created correctly with a specific destroy function");

    print_test(list_is_empty(l), "The list must be empty before inserting any struct");
    print_test(list_insert_first(l, my_struct1), "The struct can be inserted correctly");
    print_test(list_insert_first(l, my_struct2), "The struct can be inserted correctly");
    print_test(!list_is_empty(l), "The list must not be empty after inserting the structs");

    ptr = (MyStruct*)list_delete_first(l);
    print_test(strcmp(ptr->string, string2) == 0, "The struct that was stored at the first position is the last one added");
    print_test(ptr->integer == integer2, "The struct data was stored correctly");
    struct_destroy(ptr);
    ptr = NULL;

    print_test(!list_is_empty(l), "There must still be a struct left");
    
    ptr = (MyStruct*)list_get_first(l);
    print_test(!list_is_empty(l), "Peeking at the front of a list with struct types does not make it empty");
    print_test(strcmp(ptr->string, string1) == 0, "The new struct at the first position is the first one added");
    print_test(ptr->integer == integer1, "The struct data was stored correctly");

    list_destroy(l);
}

int main(void) {
    test_new_list();
    test_destroy();
    test_list_one_element();
    test_insert_first_is_equal_to_insert_last_for_an_empty_list();
    test_delete_one_element_multiple_appearances();
    test_insert_at_the_end();
    test_insert_at_the_start();
    test_emptied_list();
    test_sum_all_with_internal_iterator();
    test_cut_condition_with_internal_iterator();
    test_bulk_internal_iterator();

    test_iterator_for_empty_list();
    test_bulk_iterate_trough_a_list();
    test_iteration_insert_first_position();
    test_iteration_insert_last_position();
    test_iteration_insert_middle();
    test_iteration_delete_first();

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

bool sum_all(void* elem, void* extra) {
    *(int*)extra += *(int*)elem;
    return true;
}

bool cut_condition(void* elem, void* extra) {
    *(int*)extra += 1;
    return *(int*)elem % 3 != 0;
}

bool copy_list(void* elem, void* extra) {
    list_insert_last(*(List*)extra, (int*)elem);
    return true;
}