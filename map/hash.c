#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "map.h"

#define INITIAL_CAPACITY 19
#define VARIATION_CAPACITY 2
#define MIN_CHARGE_FACTOR 0.15
#define MAX_CHARGE_FACTOR 0.65

/******************** structure definition ********************/ 

typedef enum {
    EMPTY = 0,
    TAKEN,
    DELETED
} state_t;

typedef struct pair {
    char *key;
    void *value;
    state_t state;
} pair_t;

struct hash_t {
    pair_t *table;
    size_t capacity;
    size_t size;
    size_t deleted;
    destroy_func_t destroy;
};

struct hash_iter_t {
    Map hash;
    size_t current_index;
};

/******************** static functions declarations ********************/ 

static pair_t *hash_table_create(size_t capacity);
static void hash_table_destroy(pair_t *table, size_t capacity, destroy_func_t value_destroy);
static bool hash_table_resize(Map hash, size_t new_capacity);
static size_t hash_search(Map hash, const char *key);
static uint64_t hash_expected_index(Map hash, const char *key);
static uint64_t hash_fnv(const uint8_t *bytes);
static void next_iter_index(MapIterator iter);
static char *strdup(const char *src);

/******************** Map operations definitions ********************/

Map map_create(destroy_func_t value_destroy) {
    Map hash = (Map)malloc(sizeof(struct hash_t));
    if (hash == NULL) return NULL;

    hash->table = hash_table_create(INITIAL_CAPACITY);
    if (hash->table == NULL) {
        free(hash);
        return NULL;
    }

    hash->capacity = INITIAL_CAPACITY;
    hash->size = 0;
    hash->deleted = 0;
    hash->destroy = value_destroy;

    return hash;
}

void map_destroy(Map hash) {
    if (hash == NULL) return;

    hash_table_destroy(hash->table, hash->capacity, hash->destroy);
    free(hash);
}

size_t map_size(Map hash) {
    return hash != NULL ? hash->size : 0;
}

bool map_put(Map hash, char *key, void *value) {
    if (hash == NULL) return false;

    float charge_factor = (float)(hash->size + hash->deleted) / (float)hash->capacity;
    if (charge_factor > MAX_CHARGE_FACTOR) if (!hash_table_resize(hash, hash->capacity * VARIATION_CAPACITY)) return false;

    size_t index = hash_search(hash, key);

    if (hash->table[index].state == EMPTY) {
        hash->table[index].key = strdup(key);
        if (hash->table[index].key == NULL) return false;
        hash->size++;
        hash->table[index].state = TAKEN;
    } else if (hash->table[index].state == TAKEN && hash->destroy != NULL) (hash->destroy)(hash->table[index].value);
    hash->table[index].value = value;

    return true;
}

bool map_contains(Map hash, const char *key) {
    return hash != NULL && hash->table[hash_search(hash, key)].state == TAKEN;
}

void *map_get(Map hash, const char *key) {
    if (hash == NULL) return NULL;
    
    size_t index = hash_search(hash, key);

    return hash->table[index].state == TAKEN ? hash->table[index].value : NULL;
}

void *map_remove(Map hash, char *key) {
    if (hash == NULL) return NULL;

    size_t index = hash_search(hash, key);
    if (hash->table[index].state != TAKEN) return NULL;

    hash->size--;
    hash->deleted++;
    hash->table[index].state = DELETED;
    void *deleted = hash->table[index].value;
    
    float charge_factor = (float)hash->size / (float)hash->capacity;
    if (charge_factor < MIN_CHARGE_FACTOR && hash->capacity >= INITIAL_CAPACITY * VARIATION_CAPACITY) if (!hash_table_resize(hash, hash->capacity / VARIATION_CAPACITY)) return NULL;

    return deleted;
}

void map_for_each(Map hash, visit_func_t visit, void *extra) {
    if (hash == NULL) return;
    
    pair_t current;
    for (size_t i = 0 ; i < hash->capacity ; i++) {
        current = hash->table[i];
        if (current.state == TAKEN && !visit(current.key, current.value, extra)) break;
    }
}

/******************** Map Iterator operations definitions ********************/

MapIterator map_iter_create(Map hash) {
    if (hash == NULL) return NULL;

    MapIterator iter = (MapIterator)malloc(sizeof(struct hash_iter_t));
    if (iter == NULL) return NULL;

    iter->hash = hash;
    iter->current_index = 0;
    next_iter_index(iter);

    return iter;
}

void map_iter_destroy(MapIterator iter) {
    free(iter);
}

bool map_iter_has_next(const MapIterator iter) {
    return iter != NULL && iter->current_index < iter->hash->capacity;
}

bool map_iter_next(MapIterator iter) {
    if (iter == NULL || !map_iter_has_next(iter)) return false;

    iter->current_index++;
    next_iter_index(iter);

    return true;
}

const char *map_iter_get_current(const MapIterator iter) {
    return iter != NULL && map_iter_has_next(iter) ? iter->hash->table[iter->current_index].key : NULL;
}

/******************** static functions definitions ********************/

static pair_t *hash_table_create(size_t capacity) {
    pair_t *table = (pair_t*)malloc(capacity * sizeof(pair_t));
    if (table == NULL) return NULL;

    for (size_t i = 0 ; i < capacity ; i++) {
        table[i].state = EMPTY;
        table[i].key = NULL;
        table[i].value = NULL;
    }

    return table;
}

static void hash_table_destroy(pair_t *table, size_t capacity, destroy_func_t value_destroy) {
    pair_t current;

    for (size_t i = 0 ; i < capacity ; i++) {
        current = table[i];
        if (current.state != EMPTY) free(current.key); 
        if (current.state == TAKEN && value_destroy != NULL) (value_destroy)(current.value);
    }

    free(table);
}

static bool hash_table_resize(Map hash, size_t new_capacity) {
    pair_t *old_table = hash->table;
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
        if (old_table[i].state == TAKEN) {
            bool ok = map_put(hash, old_table[i].key, old_table[i].value);
            if (!ok) return false;
            old_table[i].state = DELETED;
        }
    }
    hash_table_destroy(old_table, old_capacity, hash->destroy);

    return true;
}

static size_t hash_search(Map hash, const char *key) {
    size_t index = hash_expected_index(hash, key);
    pair_t current;

    for ( ; hash->table[index].state != EMPTY ; index = (index+1) % hash->capacity) {
        current = hash->table[index];
        if (current.state == TAKEN && strcmp(current.key, key) == 0) return index;
    }

    return index;
}

static uint64_t hash_expected_index(Map hash, const char *key) {
    const uint8_t *key_bytes = (const uint8_t*)key;
    uint64_t index = hash_fnv(key_bytes);

    return index % hash->capacity;
}

static uint64_t hash_fnv(const uint8_t *bytes) {
    uint64_t h = 2166136261UL;

    for (int i = 0 ; bytes[i] != '\0' ; i++) {
        h ^= bytes[i];
        h *= 16777619;
    }

    return h;
}

static void next_iter_index(MapIterator iter) {
    while (map_iter_has_next(iter) && iter->hash->table[iter->current_index].state != TAKEN) iter->current_index++;
}

static char *strdup(const char *src) {
    char *string = (char*)malloc((strlen(src)+1) * sizeof(char));
    if (string == NULL) return NULL;
    strcpy(string, src);

    return string;
}