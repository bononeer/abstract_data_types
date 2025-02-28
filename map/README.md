# ADT Map

A data structure that stores key-value pairs. With a given key, you can get its value in constant time complexity, as well as update it or remove it. The keys are unique and **not** sorted.

For the given interface, the keys for the ADT Map must always be strings (char*), but the values might be of any data type (void*).

## Operations

```c
/* You may change the original type to the name you give 
to the structure that implements ADT Map 
In this case: hash_t* ; hash_iter_t */
```

### Map

```c
// A data structure that stores `key-value` pairs.
typedef struct hash_t* Map;

/* Returns an instance of an empty map. 

PRE:
- `value_destroy` function is the way the value of every `key-value` pair must
be destroyed. If NULL is given, it will not free the memory of the value. 

POST:
- if there is not enough memory for the map, the function will return NULL. */
Map map_create(void (*value_destroy)(void* value));

/* Frees the memory where the map is allocated */
void map_destroy(Map map);

/* Returns the amount of pairs mapped */
size_t map_size(Map map);

/* If the key is not stored in the map, adds the `key-value` pair to the map; otherwise, 
updates the value of the pair.

POST:
- Returns true if the item was successfully added to the map, and false if there was an 
issue with the operation. */
bool map_put(Map map, char* key, void* value);

/* Returns true if the key is stored in the map, false if not. */
bool map_contains(Map map, const char* key);

/* Return the value of the pair with the given key.

POST:
- If the key is not stored in the map, the function returns NULL.
- A void pointer will be returned, the user should cast it to the right type. */
void* map_get(Map map, const char* key);

/* Remove and return the value of the pair with the given key. 

POST:
- If the key is not stored in the map, the function returns NULL.
- If the memory was allocated previously, the returned element should be freed when not needed anymore.
- A void pointer will be returned, the user should cast it to the right type. */
void* map_remove(Map map, char* key);

/* The internal iterator of the map. Iterates through the pairs of the map in no order, 
applying the visit function to each one. If `visit(key, value, ...)` return false, the 
iteration stops. 

PRE:
- `visit` is a function that returns a boolean value, it receives the current pair in
the iteration and an extra parameter that could be NULL.
- `extra` is an extra parameter that could be NULL; if it is not, it must be passed to the
`visit` function. */
void map_for_each(Map map, bool visit(const char* key, void* value, void* extra), void* extra);
```

### Exteranl Iterator

```c
// The structure for the ADT Map's external iterator
typedef struct map_iter_t* MapIterator;

/* Returns an instance of an external iterator for the map. 

POST:
- if there is not enough memory for the iterator, the function will return NULL.*/
MapIterator map_iter_create(Map map);

/* Frees the memory where the map iterator is allocated. */
void map_iter_destroy(MapIterator iter);

/* Returns true if there are pairs left to iterate through, false if not */
bool map_iter_has_next(const MapIterator iter);

/* Advances the iteration to the next pair.

POST:
- Returns true if the action was successful, false if not. */
bool map_iter_next(MapIterator iter);

/* Returns the key of the current pair at the iteration. 

POST:
- If there are no elements left to iterate through, a NULL pointer will be returned.
- The key returned should not be modified nor have its memory freed. */
const char* map_iter_get_current(const MapIterator iter);
```
