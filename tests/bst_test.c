#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bst/bst.h"
#include "../bst/stack.h"
#include "assert_msg.h"

typedef struct {
    char* string;
    int integer;
} MyStruct;

static void print_test(bool, const char*);
static MyStruct* struct_create(char* string, int integer);
static void struct_destroy(void* value);
static bool sum_values(const char *key, void *value, void *extra);
static bool smaller_than_pi(const char *key, void *value, void *extra);
static bool sum_key_length(const char *key, void *value, void *extra);
static bool ordered_sums(const char *key, void *value, void *extra);
static int atoicmp(const char *key1, const char *key2);

static void test_new_bst(void) {
    printf("TEST: A newly created Binary Search Tree works as expected.\n");

    BST bst = bst_create(strcmp, NULL);

    print_test(bst != NULL, "Create a new binary search tree");
    print_test(bst_size(bst) == 0, "A newly created bst must be empty");

    print_test(bst_remove(bst, "Hello World") == NULL, "An empty bst returns NULL, for any key, if it tries to remove");
    print_test(bst_remove(bst, "totally_stored_key") == NULL, "An empty bst returns NULL, for any key, if it tries to remove");
    print_test(bst_get(bst, "MyKey") == NULL, "An empty bst returns NULL, for any key, if it tries to get a value");
    print_test(bst_get(bst, "Hi my name is: ") == NULL, "An empty bst returns NULL, for any key, if it tries to get a value");
    print_test(!bst_contains(bst, "compared string"), "An empty bst returns NULL, for any key, if it tries to check is a key is stored");
    print_test(!bst_contains(bst, "i dont know"), "An empty bst returns NULL, for any key, if it tries to check is a key is stored");

    bst_destroy(bst);
}

static void test_destroy(void) {    // Check with valgrind
    printf("TEST: Check that the memeory of the pairs in the bst are freed correctly when using the destroy operation\n");

    BST bst1 = bst_create(strcmp, NULL);
    char *key1 = "A";
    int num = 2;

    print_test(bst_put(bst1, key1, &num), "The pair was stored correctly"); // No allocs for element's memory

    bst_destroy(bst1);

    BST bst2 = bst_create(strcmp, free);
    char *key2 = "B";
    int *num2 = (int*)malloc(sizeof(int));
    print_test(num2 != NULL, "Memory Error");
    *num2 = 3;

    print_test(bst_put(bst2, key2, num2), "The pair was stored correctly"); // Element with allocated memory
    
    bst_destroy(bst2);
}

static void test_bst_one_pair(void) {
    printf("TEST: Put a pair, check if it is contained, get its value, update it and remove the pair\n");

    BST bst = bst_create(strcmp, NULL);
    char* key = "Hi";
    unsigned long value1 = 835718320957382, value2 = 3215613907536;
    void* ptr = NULL;

    print_test(bst_size(bst) == 0, "A newly created bst should not have any pairs stored");
    print_test(!bst_contains(bst, key), "The bst must not contain a key that has not been put yet");

    print_test(bst_put(bst, key, &value1), "The pair is stored correctly");
    print_test(bst_size(bst) == 1, "The size of the bst goes up by one after storing a pair");
    print_test(bst_contains(bst, key), "The key was found in the bst");

    ptr = bst_get(bst, key);
    print_test(*(unsigned long*)ptr == value1, "The pair has the correct value asociated with the key");

    print_test(bst_put(bst, key, &value2), "The pair's value is updated correctly");
    print_test(bst_size(bst) == 1, "The size does not change after updating a pair's value");
    print_test(bst_contains(bst, key), "The key is still found in the bst after updating the pair's value");

    ptr = NULL;
    ptr = bst_get(bst, key);
    print_test(*(unsigned long*)ptr != value1, "The value of the pair is not the old one");
    print_test(*(unsigned long*)ptr == value2, "The value of the pair is the new one");

    ptr = NULL;
    ptr = bst_remove(bst, key);
    print_test(ptr != NULL, "The pair was removed correctly");
    print_test(bst_size(bst) == 0, "The size of the bst goes down by one");
    print_test(!bst_contains(bst, key), "The key is not contained in the bst");

    print_test(*(unsigned long*)ptr != value1, "The value obtained after removing the pair is not the old one");
    print_test(*(unsigned long*)ptr == value2, "The value obtained after removing the pair is the new one");

    bst_destroy(bst);
}

static void test_good_amount_of_pairs(void) {
    printf("TEST: Store a good amount of pairs in the bst, check that the values are correct, changes them and then removes all the elements until the bst is empty\n");

    BST bst = bst_create(strcmp, free);
    void *ptr = NULL;
    char current_key[10];
    int pairs[AMOUNT];

    srand(2050);
    for (int i = 0 ; i < AMOUNT ; i++) pairs[i] = rand();

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        int *value = (int*)malloc(sizeof(int));
        if (value == NULL) { bst_destroy(bst) ; print_test(value != NULL, "Memory Error"); }
        *value = pairs[i];
        bst_put(bst, current_key, value);
    }
    print_test(bst_size(bst) == AMOUNT, "The amount of stored pairs is correct");

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        ptr = bst_get(bst, current_key);
        print_test(ptr != NULL, "The value of the pair was stored correctly");
        print_test(*(int*)ptr == pairs[i], "The value asociated with the keys is stored correctly for a good amount of pairs");
    }
    ptr = NULL;

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        int* times_two = (int*)malloc(sizeof(int));
        if (times_two == NULL) { bst_destroy(bst) ; print_test(times_two != NULL, "Memory Error"); }
        *times_two = pairs[i] * 2;
        bst_put(bst, current_key, times_two);
    }
    ptr = NULL;

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        ptr = bst_get(bst, current_key);
        print_test(ptr != NULL, "The value of the pair was stored correctly");
        print_test(*(int*)ptr == pairs[i]*2, "The value asociated with the keys is updated correctly for a good amount of pairs");
    }
    ptr = NULL;

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        ptr = bst_remove(bst, current_key);
        print_test(*(int*)ptr == pairs[i]*2, "The value asociated with the keys is removed correctly for a good amount of pairs");
        free(ptr);
    }
    print_test(bst_size(bst) == 0, "The bst is empty after deleting all its pairs");

    bst_destroy(bst);
}

static void test_huge_amount_of_pairs(void) {
    printf("TEST: Store a huge amount of pairs in the bst, check that the values are correct, changes them and then removes all the elements until the bst is empty\n");

    BST bst = bst_create(strcmp, free);
    void *ptr = NULL;
    char current_key[10];
    int pairs[BULK_AMOUNT];

    srand(2050);
    for (int i = 0 ; i < BULK_AMOUNT ; i++) pairs[i] = rand();

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        int *value = (int*)malloc(sizeof(int));
        if (value == NULL) { bst_destroy(bst) ; print_test(value != NULL, "Memory Error"); }
        *value = pairs[i];
        bst_put(bst, current_key, value);
    }
    print_test(bst_size(bst) == BULK_AMOUNT, "The amount of stored pairs is correct");

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        ptr = bst_get(bst, current_key);
        print_test(ptr != NULL, "The value of the pair was stored correctly");
        print_test(*(int*)ptr == pairs[i], "The value asociated with the keys is stored correctly for a huge amount of pairs");
    }
    ptr = NULL;

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        int* times_two = (int*)malloc(sizeof(int));
        if (times_two == NULL) { bst_destroy(bst) ; print_test(times_two != NULL, "Memory Error"); }
        *times_two = pairs[i] * 2;
        bst_put(bst, current_key, times_two);
    }
    ptr = NULL;

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        ptr = bst_get(bst, current_key);
        print_test(ptr != NULL, "The value of the pair was stored correctly");
        print_test(*(int*)ptr == pairs[i]*2, "The value asociated with the keys is updated correctly for a huge amount of pairs");
    }
    ptr = NULL;

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        ptr = bst_remove(bst, current_key);
        print_test(*(int*)ptr == pairs[i]*2, "The value asociated with the keys is removed correctly for a huge amount of pairs");
        free(ptr);
    }
    print_test(bst_size(bst) == 0, "The bst is empty after deleting all its pairs");

    bst_destroy(bst);
}

static void test_emptied_bst(void) {
    printf("TEST: Verifies that an emptied bst acts like a newly created bst\n");

    BST bst = bst_create(strcmp, free);
    void* ptr = NULL;
    char current_key[10];
    bool ok = true;
    int pairs[AMOUNT];

    srand(2050);
    for (int i = 0 ; i < AMOUNT ; i++) pairs[i] = rand();

    for (int i = 0 ; i < AMOUNT && ok ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        int* value = (int*)malloc(sizeof(int));
        if (value == NULL) { bst_destroy(bst) ; print_test(value != NULL, "Memory Error"); }
        *value = pairs[i];
        bst_put(bst, current_key, value);
        ptr = bst_get(bst, current_key);
        ok = *(int*)ptr == pairs[i] && bst_size(bst) > 0; 
    }
    print_test(ok, "A good amount of pairs can be stored in the bst");

    for (int i = AMOUNT - 1 ; i >= 0 && ok ; i--) {
        sprintf(current_key, "%d", pairs[i]);
        ptr = bst_remove(bst, current_key);
        ok = *(int*)ptr == pairs[i];
        free(ptr);
    }

    print_test(bst_size(bst) == 0, "An emptied bst must be empty");

    print_test(bst_remove(bst, current_key) == NULL, "An emptied bst returns NULL, for any key, if it tries to remove");
    print_test(bst_get(bst, current_key) == NULL, "An emptied bst returns NULL, for any key, if it tries to get a value");
    print_test(!bst_contains(bst, current_key), "An emptied bst returns NULL, for any key, if it tries to check is a key is stored");

    bst_destroy(bst);
}

void test_other_cmp_function(void) {
    printf("Check that a bst can work with a different cmp function");

    BST bst = bst_create(atoicmp, free);
    void *ptr = NULL;
    char current_key[10];
    int pairs[AMOUNT];

    srand(2050);
    for (int i = 0 ; i < AMOUNT ; i++) pairs[i] = rand();

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        int *value = (int*)malloc(sizeof(int));
        if (value == NULL) { bst_destroy(bst) ; print_test(value != NULL, "Memory Error"); }
        *value = pairs[i];
        bst_put(bst, current_key, value);
    }
    print_test(bst_size(bst) == AMOUNT, "The amount of stored pairs is correct with a different cmp function");

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        ptr = bst_get(bst, current_key);
        print_test(ptr != NULL, "The value of the pair was stored correctly with a different cmp function");
        print_test(*(int*)ptr == pairs[i], "The value asociated with the keys is stored correctly for a good amount of pairs even with a different cmp function");
    }
    ptr = NULL;

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        int* times_two = (int*)malloc(sizeof(int));
        if (times_two == NULL) { bst_destroy(bst) ; print_test(times_two != NULL, "Not enough memory"); }
        *times_two = pairs[i] * 2;
        bst_put(bst, current_key, times_two);
    }
    ptr = NULL;

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        ptr = bst_get(bst, current_key);
        print_test(ptr != NULL, "The value of the pair was stored correctly with a different cmp function");
        print_test(*(int*)ptr == pairs[i]*2, "The value asociated with the keys is updated correctly for a good amount of pairs even with a different cmp function");
    }
    ptr = NULL;

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        ptr = bst_remove(bst, current_key);
        print_test(*(int*)ptr == pairs[i]*2, "The value asociated with the keys is removed correctly for a good amount of pairs even with a different cmp function");
        free(ptr);
    }
    print_test(bst_size(bst) == 0, "The bst is empty after deleting all its pairs with a different cmp function");

    bst_destroy(bst);
}

void test_key_reutilization(void) {
    printf("TEST: Check that there is no problem when storing a pair with a key from an already deleted one\n");

    BST bst = bst_create(strcmp, NULL);
    char *key = "hello", *value1 = "world!", *value2 = "earth!";
    void* ptr = NULL;

    bst_put(bst, key, value1);
    ptr = bst_remove(bst, key);

    print_test(bst_size(bst) == 0, "The size of the bst must be 0 after removing its only pair");
    print_test(!bst_contains(bst, key), "The key is not contained in the bst");

    bst_put(bst, key, value2);

    print_test(bst_size(bst) == 1, "The size of the bst must be 1 after storing a new pair");
    print_test(bst_contains(bst, key), "The key is contained in the bst");

    ptr = bst_get(bst, key);
    print_test(strcmp((char*)ptr, value1) != 0, "The value asociated to the key is not the one stored the first time");
    print_test(strcmp((char*)ptr, value2) == 0, "The value asociated to the key is the one stored after reutilizing the key");

    bst_destroy(bst);
}

void test_internal_iterator_no_cut_condition(void) {
    printf("TEST: Verifies that the internal iterator with a visit function that does not have a cut condition works fine\n");

    BST bst = bst_create(strcmp, free);
    int expected_sum = 0, iterator_sum = 0;
    char current_key[10];
    int pairs[AMOUNT];

    srand(2050);
    for (int i = 0 ; i < AMOUNT ; i++) pairs[i] = rand();

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        int* value = (int*)malloc(sizeof(int));
        if (value == NULL) { bst_destroy(bst) ; print_test(value != NULL, "Memory Error"); }
        *value = pairs[i];
        bst_put(bst, current_key, value);
        expected_sum += pairs[i];
    }

    bst_for_each(bst, sum_values, &iterator_sum);
    print_test(expected_sum == iterator_sum, "The iterator went through all the pairs in the bst");

    bst_destroy(bst);
}

void test_internal_iterator_no_ranges(void) {
    printf("TEST: Check that the internal iterator and the internal iterator with ranges not specified are the same. Also tests that to not specify the ranges and specify the smaller and bigger keys have the same result\n");

    BST bst = bst_create(atoicmp, free);
    float iterator_sum = 0, iterator_range_sum = 0, iterator_null_ranges_sum = 0;
    char current_key[10];
    float pairs[AMOUNT], *smaller = NULL, *bigger = NULL;

    srand(2050);
    for (int i = 0 ; i < AMOUNT ; i++) {
        pairs[i] = (float)rand() * 0.00000000125f;
        if (smaller == NULL || *smaller > pairs[i]) smaller = &pairs[i];
        if (bigger == NULL || *bigger < pairs[i]) bigger = &pairs[i];
    }

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%f", pairs[i]);
        float *value = (float*)malloc(sizeof(float));
        if (value == NULL) { bst_destroy(bst) ; print_test(value != NULL, "Memory Error"); }
        *value = pairs[i];
        bst_put(bst, current_key, value);
    }

    bst_for_each(bst, smaller_than_pi, &iterator_sum);
    
    bst_for_each_range(bst, NULL, NULL, smaller_than_pi, &iterator_null_ranges_sum);
    
    char start[10], end[10];
    sprintf(start, "%f", *smaller);
    sprintf(end, "%f", *bigger);
    bst_for_each_range(bst, start, end, smaller_than_pi, &iterator_range_sum);

    print_test(iterator_sum == iterator_null_ranges_sum, "When the ranges are not specified, the result is the same as the regular internal iterator");
    print_test(iterator_sum == iterator_range_sum, "When the specified keys are the smaller and bigger respectively, the result is the same as the regular internal iterator");

    print_test(iterator_null_ranges_sum == iterator_range_sum, "Both ranged iterations have the same result");

    bst_destroy(bst);
}

void test_internal_iterator_range_no_cut_condition(void) {
    printf("TEST: Verifies that the ranged internal iterator works correctly for a bst\n");

    BST bst = bst_create(strcmp, NULL);
    char* key_values[] = {"AAAFirst Key", "BBB", "CCCMyKey", "DDDthis is a long key", "EEEthelastone"};
    size_t expected = 0, actual = 0;

    bst_put(bst, key_values[2], key_values[2]);
    bst_put(bst, key_values[0], key_values[0]);
    bst_put(bst, key_values[1], key_values[1]);
    bst_put(bst, key_values[3], key_values[3]);
    bst_put(bst, key_values[4], key_values[4]);

    for (int i = 0 ; i < 5 ; i++) expected += strlen(key_values[i]);
    bst_for_each_range(bst, key_values[0], key_values[4], sum_key_length, &actual); // Full iteration
    print_test(expected == actual, "When all the pairs are visited, the ranged internal iteration works as expected");

    expected = strlen(key_values[0]) + strlen(key_values[1]) + strlen(key_values[2]); 
    actual = 0;
    bst_for_each_range(bst, key_values[0], key_values[2], sum_key_length, &actual); // Starting keys iteration
    print_test(expected == actual, "When some pairs from the start are visited, the ranged internal iteration works as expected");

    expected = strlen(key_values[2]) + strlen(key_values[3]) + strlen(key_values[4]); 
    actual = 0;
    bst_for_each_range(bst, key_values[2], key_values[4], sum_key_length, &actual); // Ending keys iteration
    print_test(expected == actual, "When some pairs from the end are visited, the ranged internal iteration works as expected");

    expected = strlen(key_values[1]) + strlen(key_values[2]) + strlen(key_values[3]); 
    actual = 0;
    bst_for_each_range(bst, key_values[1], key_values[3], sum_key_length, &actual); // Middle keys iteration
    print_test(expected == actual, "When some pairs from the middle are visited, the ranged internal iteration works as expected");

    bst_destroy(bst);
}

void test_internal_iterator_one_range(void) {
    printf("TEST: Verifies that the ranged internal iterator works correctly when specifying only one limit\n");

    BST bst = bst_create(strcmp, NULL);
    char* key_values[] = {"AAAFirst Key", "BBB", "CCCMyKey", "DDDthis is a long key", "EEEthelastone"};
    size_t expected = 0, actual = 0;

    for (int i = 0 ; i < 5 ; i++) bst_put(bst, key_values[i], key_values[i]);

    expected = strlen(key_values[0]) + strlen(key_values[1]) + strlen(key_values[2]); 
    bst_for_each_range(bst, NULL, key_values[2], sum_key_length, &actual); // Starting keys iteration
    print_test(expected == actual, "When the the range has a NULL 'from' key, the iteration goes from the smallest key");

    expected = strlen(key_values[3]) + strlen(key_values[4]); 
    actual = 0;
    bst_for_each_range(bst, key_values[3], NULL, sum_key_length, &actual); // Ending keys iteration
    print_test(expected == actual, "When the the range has a NULL 'to' key, the iteration goes up until the biggest key");

    bst_destroy(bst);
}

void test_iterator_for_empty_bst(void) {
    printf("TEST: An iterator created for an empty bst should act as an finished iterator\n");

    BST bst = bst_create(atoicmp, NULL);
    BSTIterator iter = bst_iter_create(bst);

    print_test(iter != NULL, "Create a new BST Iterator");
    print_test(!bst_iter_has_next(iter), "An iterator for an empty bst must not have pairs to iterate through");
    print_test(!bst_iter_next(iter), "The iterator can not advance to the next pair as there are none to iterate through");
    print_test(bst_iter_get_current(iter) == NULL, "Trying to get the current key must return NULL if there are no pairs to iterate through");

    bst_iter_destroy(iter);
    bst_destroy(bst);
}

void test_ranged_iterator_for_empty_bst(void) {
    printf("TEST: A ranged iterator created for an empty bst should act as an finished iterator\n");

    BST bst = bst_create(atoicmp, NULL);
    BSTIterator iter = bst_iter_range_create(bst, NULL, NULL);

    print_test(iter != NULL, "Create a new BST Iterator with NULL ranges");
    print_test(!bst_iter_has_next(iter), "An iterator with NULL ranges for an empty bst must not have pairs to iterate through");
    print_test(!bst_iter_next(iter), "The iterator with NULL ranges can not advance to the next pair as there are none to iterate through");
    print_test(bst_iter_get_current(iter) == NULL, "Trying to get the current key must return NULL if there are no pairs to iterate through");

    bst_iter_destroy(iter);
    bst_destroy(bst);
}

void test_external_iterator_no_ranges(void) {
    printf("TEST: Check that the external iterator and the external iterator with ranges not specified are the same. Also tests that to not specify the ranges and specify the smaller and bigger keys have the same result\n");

    BST bst = bst_create(atoicmp, free);
    char current_key[10];
    int pairs[AMOUNT], *smaller = NULL, *bigger = NULL;

    srand(2050);
    for (int i = 0 ; i < AMOUNT ; i++) {
        pairs[i] = rand();
        if (smaller == NULL || *smaller > pairs[i]) smaller = &pairs[i];
        if (bigger == NULL || *bigger < pairs[i]) bigger = &pairs[i];
    }

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        int *value = (int*)malloc(sizeof(int));
        if (value == NULL) { bst_destroy(bst) ; print_test(value != NULL, "Memory Error"); }
        *value = pairs[i];
        bst_put(bst, current_key, value);
    }

    BSTIterator iter = bst_iter_create(bst);
    for (int i = 0 ; i < AMOUNT ; i++) {
        print_test(bst_iter_has_next(iter), "There are still pairs to iterate through");
        const char *key = bst_iter_get_current(iter);
        print_test(bst_contains(bst, key), "The keys from the iteration are all stored in the bst");
        print_test(bst_iter_next(iter), "The iterator can advance to the next pair");
    }
    bst_iter_destroy(iter);

    iter = bst_iter_range_create(bst, NULL, NULL);
    for (int i = 0 ; i < AMOUNT ; i++) {
        print_test(bst_iter_has_next(iter), "There are still pairs to iterate through");
        const char *key = bst_iter_get_current(iter);
        print_test(bst_contains(bst, key), "The keys from the iteration are all stored in the bst");
        print_test(bst_iter_next(iter), "The iterator can advance to the next pair");
    }
    bst_iter_destroy(iter);

    char start[10], end[10];
    sprintf(start, "%d", *smaller);
    sprintf(end, "%d", *bigger);

    iter = bst_iter_range_create(bst, start, end);
    for (int i = 0 ; i < AMOUNT ; i++) {
        print_test(bst_iter_has_next(iter), "There are still pairs to iterate through");
        const char *key = bst_iter_get_current(iter);
        print_test(bst_contains(bst, key), "The keys from the iteration are all stored in the bst");
        print_test(bst_iter_next(iter), "The iterator can advance to the next pair");
    }
    bst_iter_destroy(iter);

    bst_destroy(bst);
}

void test_external_iterator_ranges(void) {
    printf("TEST: Verifies that the ranged external iterator works correctly for a bst\n");

    BST bst = bst_create(strcmp, NULL);
    char* key_values[] = {"AAAFirst Key", "BBB", "CCCMyKey", "DDDthis is a long key", "EEEthelastone"};
    size_t expected = 0, actual = 0;

    bst_put(bst, key_values[2], key_values[2]);
    bst_put(bst, key_values[0], key_values[0]);
    bst_put(bst, key_values[1], key_values[1]);
    bst_put(bst, key_values[3], key_values[3]);
    bst_put(bst, key_values[4], key_values[4]);
    BSTIterator iter;

    for (int i = 0 ; i < 5 ; i++) expected += strlen(key_values[i]);
    iter = bst_iter_range_create(bst, key_values[0], key_values[4]);
    for ( ; bst_iter_has_next(iter) ; bst_iter_next(iter)) {
        const char *key = bst_iter_get_current(iter);
        actual += strlen(key);
    }
    bst_iter_destroy(iter);

    print_test(expected == actual, "When all the pairs are visited, the ranged external iteration works as expected");
    
    expected = 0;
    for (int i = 0 ; i < 3 ; i++) expected += strlen(key_values[i]);
    actual = 0;
    iter = bst_iter_range_create(bst, key_values[0], key_values[2]);
    for ( ; bst_iter_has_next(iter) ; bst_iter_next(iter)) {
        const char *key = bst_iter_get_current(iter);
        actual += strlen(key);
    }
    bst_iter_destroy(iter);

    print_test(expected == actual, "When some pairs from the start are visited, the ranged external iteration works as expected");

    expected = 0;
    for (int i = 2 ; i < 5 ; i++) expected += strlen(key_values[i]);
    actual = 0;
    iter = bst_iter_range_create(bst, key_values[2], key_values[4]);
    for ( ; bst_iter_has_next(iter) ; bst_iter_next(iter)) {
        const char *key = bst_iter_get_current(iter);
        actual += strlen(key);
    }
    bst_iter_destroy(iter);

    print_test(expected == actual, "When some pairs from the end are visited, the ranged external iteration works as expected");

    expected = 0;
    for (int i = 1 ; i < 4 ; i++) expected += strlen(key_values[i]);
    actual = 0;
    iter = bst_iter_range_create(bst, key_values[1], key_values[3]);
    for ( ; bst_iter_has_next(iter) ; bst_iter_next(iter)) {
        const char *key = bst_iter_get_current(iter);
        actual += strlen(key);
    }
    bst_iter_destroy(iter);

    print_test(expected == actual, "When some pairs from the middle are visited, the ranged external iteration works as expected");

    bst_destroy(bst);
}

void test_external_iterator_one_range(void) {
    printf("TEST: Verifies that the ranged external iterator works correctly when specifying only one limit\n");

    BST bst = bst_create(strcmp, NULL);
    char* key_values[] = {"AAAFirst Key", "BBB", "CCCMyKey", "DDDthis is a long key", "EEEthelastone"};
    size_t expected = 0, actual = 0;

    bst_put(bst, key_values[2], key_values[2]);
    bst_put(bst, key_values[0], key_values[0]);
    bst_put(bst, key_values[1], key_values[1]);
    bst_put(bst, key_values[3], key_values[3]);
    bst_put(bst, key_values[4], key_values[4]);
    BSTIterator iter;

    for (int i = 0 ; i < 3 ; i++) expected += strlen(key_values[i]);
    iter = bst_iter_range_create(bst, NULL, key_values[2]);
    for ( ; bst_iter_has_next(iter) ; bst_iter_next(iter)) {
        const char *key = bst_iter_get_current(iter);
        actual += strlen(key);
    }
    bst_iter_destroy(iter);

    print_test(expected == actual, "When the the range has a NULL 'from' key, the iteration goes from the smallest key");

    expected = 0;
    actual = 0;
    for (int i = 0 ; i < 3 ; i++) expected += strlen(key_values[i]);
    iter = bst_iter_range_create(bst, NULL, key_values[2]);
    for ( ; bst_iter_has_next(iter) ; bst_iter_next(iter)) {
        const char *key = bst_iter_get_current(iter);
        actual += strlen(key);
    }
    bst_iter_destroy(iter);

    print_test(expected == actual, "When the the range has a NULL 'to' key, the iteration goes up until the biggest key");

    bst_destroy(bst);
}

void test_bulk_iterate_through_a_bst(void) {
    printf("TEST: Iterate through a bst with a huge amount of pairs and check that all the iterator operations work correctly\n");

    BST bst = bst_create(atoicmp, free);
    char current_key[10];
    int pairs[BULK_AMOUNT];

    srand(2050);
    for (int i = 0 ; i < BULK_AMOUNT ; i++) pairs[i] = rand();

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        sprintf(current_key, "%d", pairs[i]);
        int* value = (int*)malloc(sizeof(int));
        print_test(value != NULL, "");
        *value = pairs[i];
        bst_put(bst, current_key, value);
    }

    BSTIterator iter = bst_iter_create(bst);
    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        print_test(bst_iter_has_next(iter), "There must still be pairs to iterate through");
        const char* key = bst_iter_get_current(iter);
        print_test(bst_contains(bst, key), "The keys from the iteration are all stored in the bst");
        print_test(bst_iter_next(iter), "The iterator must be able to advance to the next pair as there are some left");
    }

    bst_iter_destroy(iter);
    bst_destroy(bst);
}

void test_bst_is_ordered(void) {
    printf("TEST: Check that the BST pairs are ordered by the cmp function given when you iterate through them\n");

    BST bst = bst_create(atoicmp, NULL);
    char* keys[] = {"1", "2", "3", "4", "5", "6", "7"};
    int values[] = {1, 2, 3, 4, 5, 6, 7};

    bst_put(bst, keys[3], &values[3]);
    bst_put(bst, keys[1], &values[1]);
    bst_put(bst, keys[5], &values[5]);
    bst_put(bst, keys[0], &values[0]);
    bst_put(bst, keys[2], &values[2]);
    bst_put(bst, keys[4], &values[4]);
    bst_put(bst, keys[6], &values[6]);

    // this bst is a balanced tree

    int sum = 0;
    bst_for_each(bst, ordered_sums, &sum);

    print_test(sum == 7, "All the elements were visited in the right order");

    BSTIterator iter = bst_iter_create(bst);
    for (int i = 0 ; i < 7 ; i++) {
        print_test(bst_iter_has_next(iter), "There must still be elements left to iterate through");
        const char *key = bst_iter_get_current(iter);
        print_test(strcmp(key, keys[i]) == 0, "The keys are in the correct order");
        bst_iter_next(iter);
    }

    bst_iter_destroy(iter);
    bst_destroy(bst);
}

void test_struct_values(void) {
    printf("TEST: Put structs into the bst and check that it works correctly\n");

    BST bst = bst_create(strcmp, *struct_destroy);
    char *string1 = "Hello World", *string2 = "This is a struct!"; 
    int integer1 = 10, integer2 = -14;
    MyStruct *my_struct1 = struct_create(string1, integer1), *my_struct2 = struct_create(string2, integer2);
    MyStruct* ptr = NULL;

    print_test(bst != NULL, "The bst is created correctly with a specific destroy function");

    print_test(bst_size(bst) == 0, "The bst must be empty before storing any struct");
    print_test(bst_put(bst, string1, my_struct1), "The struct can be stored correctly as a value");
    print_test(bst_put(bst, string2, my_struct2), "The struct can be stored correctly as a value");
    print_test(bst_size(bst) > 0, "The bst must not be empty after storing the structs");

    ptr = (MyStruct*)bst_remove(bst, string1);
    print_test(strcmp(ptr->string, string1) == 0, "The struct is asociated to its key correctly");
    print_test(ptr->integer == integer1, "The struct data was stored correctly");
    struct_destroy(ptr);
    ptr = NULL;

    print_test(bst_size(bst) == 1, "There must still be a struct left");
    
    ptr = (MyStruct*)bst_get(bst, string2);
    print_test(bst_size(bst) == 1, "Retrieving the struct value paired with the key does not change the size of the bst");
    print_test(strcmp(ptr->string, string2) == 0, "The struct is asociated to its key correctly");
    print_test(ptr->integer == integer2, "The struct data was stored correctly");

    bst_destroy(bst);
}

int main(void) {
    test_new_bst();
    test_destroy();
    test_bst_one_pair();
    test_good_amount_of_pairs();
    test_huge_amount_of_pairs();
    test_emptied_bst();
    test_key_reutilization();
    test_other_cmp_function();
    test_internal_iterator_no_cut_condition();
    test_internal_iterator_no_ranges();
    test_internal_iterator_range_no_cut_condition();
    test_internal_iterator_one_range();

    test_iterator_for_empty_bst();
    test_ranged_iterator_for_empty_bst();
    test_external_iterator_no_ranges();
    test_external_iterator_ranges();
    test_external_iterator_one_range();
    test_bulk_iterate_through_a_bst();

    test_bst_is_ordered();

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

bool sum_values(const char *key, void *value, void *extra) {
    *(int*)extra += *(int*)value;
    return true;
}

bool smaller_than_pi(const char *key, void *value, void *extra) {
    *(float*)extra += *(float*)value;
    return *(float*)extra < 3.1416;
}

bool sum_key_length(const char *key, void *value, void *extra) {
    *(size_t*)extra += strlen(key);
    return true;
}

bool ordered_sums(const char *key, void *value, void *extra) {
    *(int*)extra += 1;
    return *(int*)extra == *(int*)value;
}

int atoicmp(const char *key1, const char *key2) {
    return atoi(key1) - atoi(key2);
}