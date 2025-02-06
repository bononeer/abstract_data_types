#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "map.h"

#define INITIAL_CAPACITY 19
#define VARIATION_CAPACITY 2
#define MIN_CHARGE_FACTOR 0.15
#define MAX_CHARGE_FACTOR 0.65

/******************** structure definition ********************/ 

typedef void (*value_destroy)(void*);

typedef enum {
    EMPTY = 0,
    TAKEN,
    DELETED
} state_t;

typedef struct pair {
    char* key;
    void* value;
    state_t state;
} pair_t;

struct hash_t {
    pair_t** table;
    size_t size;
    size_t capacity;
    size_t deleted;
    value_destroy destroy;
};

/******************** static functions declarations ********************/ 

static pair_t** hash_table_create(size_t capacity);
static bool hash_table_resize(Map hash, size_t capacity);
static void hash_table_destroy(pair_t** table, size_t capacity, void value_destroy(void* elem)) ;
static size_t hash_search(Map hash, const char* key);
static uint64_t hash_get_index(Map hash, const char* key);
static uint64_t hash_fnv(const uint8_t* bytes);

/******************** Map operations definitions ********************/

Map map_create(void (*value_destroy)(void *value)) {
    Map hash = (Map)malloc(sizeof(struct hash_t));
    if (hash == NULL) return NULL;

    hash->size = 0;
    hash->capacity = INITIAL_CAPACITY;
    hash->deleted = 0;
    if (value_destroy == NULL) value_destroy = free;
    hash->destroy = value_destroy;

    pair_t** table_temp = hash_table_create(hash->capacity);
    if (table_temp == NULL) {
        free(hash);
        return NULL;
    }

    hash->table = table_temp;

    return hash;
}

void map_destroy(Map hash) {
    hash_table_destroy(hash->table, hash->capacity, hash->destroy);
    free(hash);
}

size_t map_size(Map hash) {
    return hash->size;
}

bool map_put(Map hash, char* key, void* value) {
    float charge_factor = (float)(hash->size + hash->deleted) / (float)hash->capacity;
    if (charge_factor > MAX_CHARGE_FACTOR) {
        bool ok = hash_table_resize(hash, hash->capacity * VARIATION_CAPACITY);
        if (!ok) return false;
    }

    size_t index = hash_search(hash, key);

    if (hash->table[index]->state == EMPTY) {
        hash->table[index]->key = (char*)malloc((strlen(key)+1) * sizeof(char));
        if (hash->table[index]->key == NULL) return false;
        strcpy(hash->table[index]->key, key);
        hash->table[index]->value = malloc(sizeof(void*));
        if (hash->table[index]->value == NULL) {
            free(hash->table[index]->key);
            return false;
        }
        hash->size++;
        hash->table[index]->state = TAKEN;
    }
    memcpy(hash->table[index]->value, value, sizeof(void*));

    return true;
}

bool map_contains(Map hash, const char* key) {
    size_t index = hash_search(hash, key);

    return hash->table[index]->state == TAKEN;
}

void* map_get(Map hash, const char* key) {
    size_t index = hash_search(hash, key);
    if (hash->table[index]->state != TAKEN) return NULL;

    return hash->table[index]->value;
}

void* map_remove(Map hash, char* key) {
    size_t index = hash_search(hash, key);
    if (hash->table[index]->state != TAKEN) return NULL;

    void* deleted = malloc(sizeof(void*));
    if (deleted == NULL) return NULL;
    memcpy(deleted, hash->table[index]->value, sizeof(void*));
    hash->size--;
    hash->deleted++;
    hash->table[index]->state = DELETED;

    float charge_factor = (float)hash->size / (float)hash->capacity;
    if (charge_factor < MIN_CHARGE_FACTOR && hash->capacity >= INITIAL_CAPACITY * VARIATION_CAPACITY) {
        bool ok = hash_table_resize(hash, hash->capacity / VARIATION_CAPACITY);
        if (!ok) return NULL;
    }

    return deleted;
}

void map_for_each(Map hash, bool visit(const char* key, void* value, void* extra), void* extra) {
    for (size_t i = 0 ; i < hash->capacity ; i++) {
        pair_t* pair = hash->table[i];
        if (pair->state == TAKEN && !visit(pair->key, pair->value, extra)) break;
    }
}

/******************** static functions definitions ********************/

static pair_t** hash_table_create(size_t capacity) {
    bool memory_error = false;
    pair_t** table = (pair_t**)malloc(capacity * sizeof(pair_t*));
    if (table == NULL) return NULL;
    
    size_t i = 0;
    for ( ; i < capacity && !memory_error ; i++) {
        table[i] = (pair_t*)malloc(sizeof(pair_t));
        if (table[i] == NULL) {
            memory_error = true;
            break;
        }

        table[i]->state = EMPTY;
    }

    if (!memory_error) return table;

    for (size_t j = 0 ; j < i ; j++) free(table[j]);
    free(table);

    return NULL;
}

static bool hash_table_resize(Map hash, size_t new_capacity) {
    pair_t** old_table = hash->table;
    hash->table = hash_table_create(new_capacity);
    if (hash->table == NULL) {
        hash->table = old_table;
        return false;
    }

    size_t old_capacity = hash->capacity;
    hash->capacity = new_capacity;

    hash->size = 0;
    hash->deleted = 0;

    for (size_t i = 0 ; i < old_capacity ; i++) {        
        if (old_table[i]->state == TAKEN) {
            bool ok = map_put(hash, old_table[i]->key, old_table[i]->value);
            if (!ok) return false;
        }
    }
    hash_table_destroy(old_table, old_capacity, hash->destroy);

    return true;
}

static void hash_table_destroy(pair_t** table, size_t capacity, void value_destroy(void* elem)) {
    for (size_t i = 0 ; i < capacity ; i++) {
        if (table[i]->state != EMPTY) {
            free(table[i]->key);
            value_destroy(table[i]->value);
        }
        free(table[i]);
    }
    free(table);
}

static size_t hash_search(Map hash, const char* key) {
    size_t index = hash_get_index(hash, key);
    pair_t* current;

    for ( ; hash->table[index]->state != EMPTY ; index = (index+1) % hash->capacity) {
        current = hash->table[index];
        if (current->state == TAKEN && strcmp(current->key, key) == 0) return index;
    }

    return index;
}

static uint64_t hash_get_index(Map hash, const char* key) {
    const uint8_t* key_bytes = (const uint8_t*)key;
    uint64_t index = hash_fnv(key_bytes);

    return index % hash->capacity;
}

static uint64_t hash_fnv(const uint8_t* bytes) {
    uint64_t h = 2166136261UL;

    for (int i = 0 ; bytes[i] != '\0' ; i++) {
        h ^= bytes[i];
        h *= 16777619;
    }

    return h;
}