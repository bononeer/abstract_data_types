#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../map/map.h"
#include "assert_msg.h"

typedef struct {
    char* string;
    int integer;
} MyStruct;

static void print_test(bool, const char*);
static MyStruct* struct_create(char* string, int integer);
static void struct_destroy(void* value);
static bool sum_values(const char* key, void* value, void* extra);

static void test_new_map(void) {
    printf("TEST: A newly created map works as expected.\n");

    Map m = map_create(NULL);

    print_test(m != NULL, "Create a new map");
    print_test(map_size(m) == 0, "A newly created map must be empty");

    print_test(map_remove(m, "Hello World") == NULL, "An empty map returns NULL, for any key, if it tries to remove");
    print_test(map_remove(m, "totally_stored_key") == NULL, "An empty map returns NULL, for any key, if it tries to remove");
    print_test(map_get(m, "MyKey") == NULL, "An empty map returns NULL, for any key, if it tries to get a value");
    print_test(map_get(m, "Hi my name is: ") == NULL, "An empty map returns NULL, for any key, if it tries to get a value");
    print_test(!map_contains(m, "hashable string"), "An empty map returns NULL, for any key, if it tries to check is a key is stored");
    print_test(!map_contains(m, "i dont know"), "An empty map returns NULL, for any key, if it tries to check is a key is stored");

    map_destroy(m);
}

static void test_destroy(void) {    // Check with valgrind
    printf("TEST: Check that the memeory of the pairs in the map are freed correctly when using the destroy operation\n");

    Map m1 = map_create(NULL), m2 = map_create(free);
    char *key1 = "A", *key2 = "B";
    int num = 2;
    int* another_num = (int*)malloc(sizeof(int));
    print_test(another_num != NULL, "");

    print_test(map_put(m1, key1, &num), "The pair was stored correctly"); // No allocs for element's memory
    map_destroy(m1);

    print_test(map_put(m2, key2, another_num), "The pair was stored correctly"); // Element with allocated memory
    map_destroy(m2);
}

static void test_map_one_pair(void) {
    printf("TEST: Put a pair, check if it is contained, get its value, update it and remove the pair\n");

    Map m = map_create(NULL);
    char* key = "Hi";
    unsigned long value1 = 835718320957382, value2 = 3215613907536;
    void* ptr = NULL;

    print_test(map_size(m) == 0, "A newly created map should not have any pairs stored");
    print_test(!map_contains(m, key), "The map must not contain a key that has not been put yet");

    print_test(map_put(m, key, &value1), "The pair is stored correctly");
    print_test(map_size(m) == 1, "The size of the map goes up by one after storing a pair");
    print_test(map_contains(m, key), "The key was found in the map");

    ptr = map_get(m, key);
    print_test(*(unsigned long*)ptr == value1, "The pair has the correct value asociated with the key");

    print_test(map_put(m, key, &value2), "The pair's value is updated correctly");
    print_test(map_size(m) == 1, "The size does not change after updating a pair's value");
    print_test(map_contains(m, key), "The key is still found in the map after updating the pair's value");

    ptr = NULL;
    ptr = map_get(m, key);
    print_test(*(unsigned long*)ptr != value1, "The value of the pair is not the old one");
    print_test(*(unsigned long*)ptr == value2, "The value of the pair is the new one");

    ptr = NULL;
    ptr = map_remove(m, key);
    print_test(ptr != NULL, "The pair was removed correctly");
    print_test(map_size(m) == 0, "The size of the map goes down by one");
    print_test(!map_contains(m, key), "The key is not contained in the map");

    print_test(*(unsigned long*)ptr != value1, "The value obtained after removing the pair is not the old one");
    print_test(*(unsigned long*)ptr == value2, "The value obtained after removing the pair is the new one");

    map_destroy(m);
}

static void test_good_amount_of_pairs(void) {
    printf("TEST: Store a good amount of pairs in the map, check that the values are correct, changes them and then removes all the elements until the map is empty\n");

    Map m = map_create(free);
    void* ptr = NULL;
    char current_key[3];

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", i);
        int* value = (int*)malloc(sizeof(int));
        print_test(value != NULL, "");
        *value = i;
        map_put(m, current_key, value);
    }
    print_test(map_size(m) == AMOUNT, "The amount of stored pairs is correct");

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", i);
        ptr = map_get(m, current_key);
        print_test(ptr != NULL, "The value of the pair was stored correctly");
        print_test(*(int*)ptr == i, "The value asociated with the keys is stored correctly for a good amount of pairs");
    }
    ptr = NULL;

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", i);
        int* times_two = (int*)malloc(sizeof(int));
        print_test(times_two != NULL, "");
        *times_two = i * 2;
        map_put(m, current_key, times_two);
    }
    ptr = NULL;

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", i);
        ptr = map_get(m, current_key);
        print_test(ptr != NULL, "The value of the pair was stored correctly");
        print_test(*(int*)ptr == i*2, "The value asociated with the keys is updated correctly for a good amount of pairs");
    }
    ptr = NULL;

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", i);
        ptr = map_remove(m, current_key);
        print_test(*(int*)ptr == i*2, "The value asociated with the keys is removed correctly for a good amount of pairs");
        free(ptr);
    }
    print_test(map_size(m) == 0, "The map is empty after deleting all its pairs");

    map_destroy(m);
}

static void test_huge_amount_of_pairs(void) {
    printf("TEST: Store a huge amount of pairs in the map, check that the values are correct, changes them and then removes all the elements until the map is empty\n");

    Map m = map_create(free);
    void* ptr = NULL;
    char current_key[5];

    for (long i = 0 ; i < BULK_AMOUNT ; i++) {
        sprintf(current_key, "%ld", i);
        long* value = (long*)malloc(sizeof(long));
        print_test(value != NULL, "");
        *value = i;
        map_put(m, current_key, value);
    }
    print_test(map_size(m) == BULK_AMOUNT, "The amount of stored pairs is correct");

    for (long i = 0 ; i < BULK_AMOUNT ; i++) {
        sprintf(current_key, "%ld", i);
        ptr = map_get(m, current_key);
        print_test(ptr != NULL, "The value of the pair was stored correctly");
        print_test(*(long*)ptr == i, "The value asociated with the keys is stored correctly for a good amount of pairs");
    }
    ptr = NULL;

    for (long i = 0 ; i < BULK_AMOUNT ; i++) {
        sprintf(current_key, "%ld", i);
        long* times_two = (long*)malloc(sizeof(long));
        print_test(times_two != NULL, "");
        *times_two = i * 2;
        map_put(m, current_key, times_two);
    }
    ptr = NULL;

    for (long i = 0 ; i < BULK_AMOUNT ; i++) {
        sprintf(current_key, "%ld", i);
        ptr = map_get(m, current_key);
        print_test(ptr != NULL, "The value of the pair was stored correctly");
        print_test(*(long*)ptr == i*2, "The value asociated with the keys is updated correctly for a good amount of pairs");
    }
    ptr = NULL;

    for (long i = 0 ; i < BULK_AMOUNT ; i++) {
        sprintf(current_key, "%ld", i);
        ptr = map_remove(m, current_key);
        print_test(*(long*)ptr == i*2, "The value asociated with the keys is removed correctly for a good amount of pairs");
        free(ptr);
    }
    print_test(map_size(m) == 0, "The map is empty after deleting all its pairs");

    map_destroy(m);
}

static void test_emptied_map(void) {
    printf("TEST: Verifies that an emptied map acts like a newly created map\n");

    Map m = map_create(free);
    void* ptr = NULL;
    char current_key[3];
    bool ok = true;

    for (int i = 0 ; i < AMOUNT && ok ; i++) {
        sprintf(current_key, "%d", i);
        int* value = (int*)malloc(sizeof(int));
        print_test(value != NULL, "");
        *value = i;
        map_put(m, current_key, value);
        ptr = map_get(m, current_key);
        ok = *(int*)ptr == i && map_size(m) > 0; 
    }
    print_test(ok, "A good amount of pairs can be stored in the map");

    for (int i = AMOUNT - 1 ; i >= 0 && ok ; i--) {
        sprintf(current_key, "%d", i);
        ptr = map_remove(m, current_key);
        ok = *(int*)ptr == i;
        free(ptr);
    }

    print_test(map_size(m) == 0, "An emptied map must be empty");

    print_test(map_remove(m, current_key) == NULL, "An emptied map returns NULL, for any key, if it tries to remove");
    print_test(map_get(m, current_key) == NULL, "An emptied map returns NULL, for any key, if it tries to get a value");
    print_test(!map_contains(m, current_key), "An emptied map returns NULL, for any key, if it tries to check is a key is stored");

    map_destroy(m);
}

void test_key_reutilization(void) {
    printf("TEST: Check that there is no problem when storing a pair with a key from an already deleted one\n");

    Map m = map_create(NULL);
    char *key = "hello", *value1 = "world!", *value2 = "earth!";
    void* ptr = NULL;

    map_put(m, key, value1);
    ptr = map_remove(m, key);

    print_test(map_size(m) == 0, "The size of the map must be 0 after removing its only pair");
    print_test(!map_contains(m, key), "The key is not contained in the map");

    map_put(m, key, value2);

    print_test(map_size(m) == 1, "The size of the map must be 1 after storing a new pair");
    print_test(map_contains(m, key), "The key is contained in the map");

    ptr = map_get(m, key);
    print_test(strcmp((char*)ptr, value1) != 0, "The value asociated to the key is not the one stored the first time");
    print_test(strcmp((char*)ptr, value2) == 0, "The value asociated to the key is the one stored after reutilizing the key");

    map_destroy(m);
}

void test_internal_iterator_no_cut_condition(void) {
    printf("TEST: Verifies that the internal iterator with a visit function that does not have a cut condition works fine\n");

    Map m = map_create(free);
    int expected_sum = 0, iterator_sum = 0;
    char current_key[3];

    for (int i = 0 ; i < AMOUNT ; i++) {
        sprintf(current_key, "%d", i);
        int* value = (int*)malloc(sizeof(int));
        print_test(value != NULL, "");
        *value = i;
        map_put(m, current_key, value);
        expected_sum += i;
    }

    map_for_each(m, sum_values, &iterator_sum);
    print_test(expected_sum == iterator_sum, "The iterator went through all the pairs in the map");

    map_destroy(m);
}

void test_iterator_for_empty_map(void) {
    printf("TEST: An iterator created for an empty map should act as an finished iterator\n");

    Map m = map_create(NULL);
    MapIterator iter = map_iter_create(m);

    print_test(iter != NULL, "Create a new Map Iterator");
    print_test(!map_iter_has_next(iter), "An iterator for an empty map must not have pairs to iterate through");
    print_test(!map_iter_next(iter), "The iterator can not advance to the next pair as there are none to iterate through");
    print_test(map_iter_get_current(iter) == NULL, "Trying to get the current key must return NULL if there are no pairs to iterate through");

    map_iter_destroy(iter);
    map_destroy(m);
}

void test_bulk_iterate_through_a_map(void) {
    printf("TEST: Iterate through a map with a huge amount of pairs and check that all the iterator operations work correctly\n");

    Map m = map_create(free);
    char current_key[5];

    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        sprintf(current_key, "%d", i);
        int* value = (int*)malloc(sizeof(int));
        print_test(value != NULL, "");
        *value = i;
        map_put(m, current_key, value);
    }

    MapIterator iter = map_iter_create(m);
    for (int i = 0 ; i < BULK_AMOUNT ; i++) {
        print_test(map_iter_has_next(iter), "There must still be pairs to iterate through");
        const char* key = map_iter_get_current(iter);
        print_test(map_contains(m, key), "The keys from the iteration are all stored in the map");
        print_test(map_iter_next(iter), "The iterator must be able to advance to the next pair as there are some left");
    }

    map_iter_destroy(iter);
    map_destroy(m);
}

void test_struct_values(void) {
    printf("TEST: Put structs into the map and check that it works correctly\n");

    Map m = map_create(*struct_destroy);
    char *string1 = "Hello World", *string2 = "This is a struct!"; 
    int integer1 = 10, integer2 = -14;
    MyStruct *my_struct1 = struct_create(string1, integer1), *my_struct2 = struct_create(string2, integer2);
    MyStruct* ptr = NULL;

    print_test(m != NULL, "The map is created correctly with a specific destroy function");

    print_test(map_size(m) == 0, "The map must be empty before storing any struct");
    print_test(map_put(m, string1, my_struct1), "The struct can be stored correctly as a value");
    print_test(map_put(m, string2, my_struct2), "The struct can be stored correctly as a value");
    print_test(map_size(m) > 0, "The map must not be empty after storing the structs");

    ptr = (MyStruct*)map_remove(m, string1);
    print_test(strcmp(ptr->string, string1) == 0, "The struct is asociated to its key correctly");
    print_test(ptr->integer == integer1, "The struct data was stored correctly");
    struct_destroy(ptr);
    ptr = NULL;

    print_test(map_size(m) == 1, "There must still be a struct left");
    
    ptr = (MyStruct*)map_get(m, string2);
    print_test(map_size(m) == 1, "Retrieving the struct value paired with the key does not change the size of the map");
    print_test(strcmp(ptr->string, string2) == 0, "The struct is asociated to its key correctly");
    print_test(ptr->integer == integer2, "The struct data was stored correctly");

    map_destroy(m);
}

int main(void) {
    
    test_new_map();
    test_destroy();
    test_map_one_pair();
    test_good_amount_of_pairs();
    test_huge_amount_of_pairs();
    test_emptied_map();
    test_key_reutilization();
    test_internal_iterator_no_cut_condition();

    test_iterator_for_empty_map();
    test_bulk_iterate_through_a_map();

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

static bool sum_values(const char* key, void* value, void* extra) {
    *(int*)extra += *(int*)value;
    return true;
}