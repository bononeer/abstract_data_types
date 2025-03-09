# ADT Map

A data structure that stores key-value pairs. With a given key, you can get its value in constant time complexity, as well as update it or remove it. The keys are unique and **not** sorted.

For the given interface, the keys for the ADT Map must always be strings (char*), but the values might be of any data type (void*).

## Struct

```c
// A data structure that stores `key-value` pairs.
typedef struct hash_t *Map;
// The external iterator for the Map
typedef struct hash_iter_t *MapIterator;
```

## Operations

### Map

```c
/* Returns an instance of an empty Map. 

PRE:
- `value_destroy` is a pointer to a `destroy_func_t` that defines how to free the memory
of the value of the pairs stored in the Map. If NULL is given, it will not free the memory 
of the values. 

POST:
- if there is not enough memory for the Map, the function will return NULL. */
Map map_create(destroy_func_t value_destroy);

/* Frees the memory where the Map is allocated. */
void map_destroy(Map map);

/* Returns the amount of pairs stored in the Map. */
size_t map_size(Map map);

/* If the key is not stored in the Map, adds the `key-value` pair to the Map; otherwise, 
updates the value of the pair.

POST:
- Returns true if the item was successfully added to the Map, and false if there was an 
issue with the operation. */
bool map_put(Map map, char *key, void *value);

/* Returns true if the key is stored in the Map, false if not. */
bool map_contains(Map map, const char *key);

/* Return the value of the pair for the given key.

POST:
- If the key is not stored in the Map, the function returns NULL. */
void *map_get(Map map, const char *key);

/* Remove and return the value of the pair with the given key. 

POST:
- If the key is not stored in the Map, the function returns NULL.
- If the memory was allocated previously, the returned element should be freed when not 
needed anymore. */
void *map_remove(Map map, char *key);

/* The internal iterator of the Map. Iterates through the pairs of the Map in no order, 
applying the visit function to each one. If `visit(key, value, ...)` return false, the 
iteration stops. 

PRE:
- `visit` is a function that returns a boolean value, it receives the current pair in
the iteration and an extra parameter that could be NULL.
- `extra` is an extra parameter that could be NULL; if it is not, it must be passed to the
`visit` function. */
void map_for_each(Map map, visit_func_t visit, void* extra);
```

### Exteranl Iterator

```c
/* Returns an instance of an external iterator for the Map. 

POST:
- if there is not enough memory for the iterator, the function will return NULL. */
MapIterator map_iter_create(Map map);

/* Frees the memory where the Map iterator is allocated. */
void map_iter_destroy(MapIterator iter);

/* Returns true if there are pairs left to iterate through, false if not. */
bool map_iter_has_next(const MapIterator iter);

/* Advances the iteration to the next pair.

POST:
- Returns true if the action was successful, false if not. */
bool map_iter_next(MapIterator iter);

/* Returns the key of the current pair at the iteration. 

POST:
- If there are no elements left to iterate through, a NULL pointer will be returned. */
const char *map_iter_get_current(const MapIterator iter);
```
