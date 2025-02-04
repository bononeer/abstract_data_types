# ADT Map

A data structure that stores key-value pairs. With a given key, you might get its value in constant time complexity. The keys are not sorted and are unique.

For the given interface, the keys for the ADT Map must always be strings (char*), but the values might be of any data type (void*).

The ADT Map is implemented with open hashing. For that reason, `hash.c` needs the interface and implementation of the ADT List.

## Operations

```c
typedef struct hash_t* Map;

/* Returns an instance of an empty map. 

POST:
- if there is not enough memory for the map, the function will return NULL. */
Map map_create();

/* Frees the memory where the map is allocated */
void map_destroy(Map map);

/* Returns the amount of elements mapped */
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
- The returned pointer should be freed when not needed anymore.
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
