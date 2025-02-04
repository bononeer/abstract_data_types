#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "map.h"
#include "../list/list.h"

#define INITIAL_CAPACITY 19
#define VARIATION_CAPACITY 2
#define MIN_CHARGE_FACTOR 0.5
#define MAX_CHARGE_FACTOR 2.5

/******************** structure definition ********************/ 

typedef struct pair {
    char* key;
    void* value;
} pair_t;

struct hash_t {
    List* table;        // An array of pair_t lists 
    size_t size;
    size_t capacity;
};

/******************** static functions declarations ********************/ 

static List* hash_table_create(size_t capacity);
static bool hash_table_resize(Map hash, size_t capacity);
/* Destroys the hash table, only frees the lists of each position of
the table up until the index given (not included). */
static void hash_table_destroy(List* table, size_t index);
/* If the key is stored in the map, it returns a list iterator with its current element 
being the place where the pair is. If not, it returns a finished iterator. */
static ListIterator hash_search(Map hash, const char* key);
/* Returns the index of the place where a pair with the given key should be 
ignoring collisions. */
static size_t hash_get_index(Map hash, const char* key);
/* Fowler-Noll-Vo, a non-cryptographic hash function created by Glenn Fowler, 
Landon Curt Noll and Kiem-Phong Vo */  
static uint64_t hash_fnv(const uint8_t* bytes);
/* Cast elem to a pair_t* and if successful, frees its memory */
static void pair_destroy(void* elem);

/******************** Map operations definitions ********************/

Map map_create() {
    Map hash = (Map)malloc(sizeof(struct hash_t));
    if (hash == NULL) return NULL;


    List* table_temp = hash_table_create(INITIAL_CAPACITY);
    if (table_temp == NULL) {
        free(hash);
        return NULL;
    }

    hash->size = 0;
    hash->capacity = INITIAL_CAPACITY;
    hash->table = table_temp;

    return hash;
}

void map_destroy(Map hash) {
    hash_table_destroy(hash->table, hash->capacity);
    free(hash);
}

size_t map_size(Map hash) {
    return hash->size;
}

bool map_put(Map hash, char* key, void* value) {
    float charge_factor = (float)hash->size / (float)hash->capacity;
    if (charge_factor > MAX_CHARGE_FACTOR) {
        bool ok = hash_table_resize(hash, hash->capacity * VARIATION_CAPACITY);
        if (!ok) return false;
    }

    ListIterator iter = hash_search(hash, key);
    if (iter == NULL) return false;  // Not enough memory to create the iterator 

    if (!list_iter_has_next(iter)) {
        pair_t* pair = (pair_t*)malloc(sizeof(struct pair));
        pair->key = key;
        pair->value = value; 

        list_iter_insert(iter, pair);
        hash->size++;
    } else {
        pair_t* current_pair = (pair_t*)list_iter_get_current(iter);
        current_pair->value = value;
    }

    list_iter_destroy(iter);

    return true;
}

bool map_contains(Map hash, const char* key) {
    ListIterator iter = hash_search(hash, key);
    if (iter == NULL) return false;  // Not enough memory to create the iterator 

    bool contains = list_iter_has_next(iter); 

    list_iter_destroy(iter);

    return contains;
}

void* map_get(Map hash, const char* key) {
    ListIterator iter = hash_search(hash, key);
    if (iter == NULL) return false;  // Not enough memory to create the iterator
    else if (!list_iter_has_next(iter)) {
        list_iter_destroy(iter);
        return NULL;
    }

    pair_t* current_pair = (pair_t*)list_iter_get_current(iter);
    
    list_iter_destroy(iter);

    return current_pair->value;
}

void* map_remove(Map hash, char* key) {
    return NULL;
}

void map_for_each(Map hash, bool visit(const char* key, void* value, void* extra), void* extra) {
    bool ok = true;

    for (size_t i = 0 ; i < hash->capacity && ok; i++) {
        ListIterator iter = list_iter_create(hash->table[i]);
        if (iter == NULL) break;

        while (list_iter_has_next(iter)) {
            pair_t* current_pair = (pair_t*)list_iter_get_current(iter);
            if (!visit(current_pair->key, current_pair->value, extra)) {
                ok = false;
                break;
            }
            list_iter_next(iter);
        }

        list_iter_destroy(iter);
    }
}

/******************** static functions definitions ********************/

static List* hash_table_create(size_t capacity) {
    List* table = (List*)malloc(capacity * sizeof(List));
    if (table == NULL) return NULL;

    for (size_t i = 0 ; i < capacity ; i++) {
        table[i] = list_create();
        if (table[i] == NULL) {
            hash_table_destroy(table, i);
            return NULL;
        }
    }
    
    return table;
}

static bool hash_table_resize(Map hash, size_t new_capacity) {
    List* old_table = hash->table;
    size_t old_capacity = hash->capacity;

    List* table_temp = hash_table_create(new_capacity);
    if (table_temp == NULL) return false;

    hash->size = 0;
    hash->capacity = new_capacity;
    hash->table = table_temp;

    for (size_t i = 0 ; i < old_capacity ; i++) {
        ListIterator iter = list_iter_create(old_table[i]);
        while (list_iter_has_next(iter)) {
            pair_t* current_pair = (pair_t*)list_iter_get_current(iter);
            map_put(hash, current_pair->key, current_pair->value);
            list_iter_next(iter);
        }
    }
    hash_table_destroy(old_table, old_capacity);

    return true;
}

static void hash_table_destroy(List* table, size_t index) {
    for (size_t i = 0 ; i < index ; i++) {
        list_destroy(table[i], pair_destroy);
    }
    free(table);
}

static ListIterator hash_search(Map hash, const char* key) {
    size_t index = hash_get_index(hash, key);

    ListIterator iter = list_iter_create(hash->table[index]);
    if (iter == NULL) return NULL;

    while(list_iter_has_next(iter)) {
        pair_t* current_pair = (pair_t*)list_iter_get_current(iter);
        if (strcmp(current_pair->key, key) == 0) break;

        list_iter_next(iter);
    }

    return iter;
}

static size_t hash_get_index(Map hash, const char* key) {
    const uint8_t* key_bytes = (const uint8_t*)key;
    uint64_t index = hash_fnv(key_bytes);

    return (size_t)index % hash->capacity;
}

static uint64_t hash_fnv(const uint8_t* bytes) {
    uint64_t h = 2166136261UL;

    for (int i = 0 ; bytes[i] != '\0' ; i++) {
        h ^= bytes[i];
        h *= 16777619;
    }

    return h;
}

static void pair_destroy(void* elem) {
    pair_t* pair = (pair_t*)elem;
    if (pair != NULL) {
        free(pair->key);
        free(pair->value);
        free(pair);    
    }
}