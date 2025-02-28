# ADT BST - Binary Search Tree

A data structure that works as a Sorted Map, it stores key-value pairs "in order". The order is decided with a given `cmp_func` in the bst creator. The operations to put, get, check if the key is contained or remove are executed in logarithmic time complexity.

## Additional Types

```c
/* A compare function that returns the following in different cases:
    - `num < 0` if `key1` is lesser than `key2`. 
    - `num == 0` if `key1` is equal to `key2`.
    - `num > 0` if `key1` is greater than `key2`. */
typedef int (*cmp_func_t)(const char *key1, const char *key2);
// A function that defines how to destroy a given element.
typedef void (*value_destroy_t)(void*);
/* A function for the internal iterators which returns a boolean value to decide if the 
iteration continues or not. It receives an `extra` parameter that can be NULL. */
typedef bool (*visit_func_t)(const char *key, void *value, void *extra);
```

## Operations

### BST

```c
// A data structure that stores `key-value` pairs with a defined order.
typedef struct bst_t* BST;

/* Returns an instance of an empty bst. 

PRE:
- `cmp` function decides the order in which the keys will be sorted inside the bst.
- `value_destroy` function is the way the value of every `key-value` pair must
be destroyed. If NULL is given, it will not free the memory of the value. 

POST:
- If cmp is NULL, the function returns NULL.
- if there is not enough memory for the bst, the function will return NULL. */
BST bst_create(cmp_func_t cmp, value_destroy_t value_destroy);

/* Frees the memory where the bst is allocated */
void bst_destroy(BST bst);

/* Returns the amount of pairs stored in the bst */
size_t bst_size(BST bst);

/* If the key is not stored in the bst, adds the `key-value` pair to the bst; otherwise, 
updates the value of the pair.

POST:
- Returns true if the item was successfully added to the bst, and false if there was an 
issue with the operation. */
bool bst_put(BST bst, char *key, void *value);

/* Returns true if the key is stored in the bst, false if not. */
bool bst_contains(BST bst, const char *key);

/* Return the value of the pair with the given key.

POST:
- If the key is not stored in the bst, the function returns NULL.
- A void pointer will be returned, the user should cast it to the right type. */
void *bst_get(BST bst, const char *key);

/* Remove and return the value of the pair with the given key. 

POST:
- If the key is not stored in the bst, the function returns NULL.
- If the memory was allocated previously, the returned element should be freed when not needed anymore.
- A void pointer will be returned, the user should cast it to the right type. */
void *bst_remove(BST bst, char *key);

/* Iterates through the pairs of the bst in order according to the cmp function, applying the visit 
function to each one. If `visit(key, value, ...)` return false, the iteration stops. 

PRE:
- `extra` is the extra parameter that is given to the visit function. */
void bst_for_each(BST bst, visit_func_t visit, void *extra);

/* Iterates through the pairs of the bst in order according to the cmp function, applying the visit 
function to each one. If `visit(key, value, ...)` return false, the iteration stops. It only iterates
through the keys that are between `from` and `to`, included.

PRE:
- `from` and `to` are two keys contained in the bst. If `from` is NULL, it iterates from the start. 
If `to` is NULL, it iterates until the end. 
- `extra` is the extra parameter that is given to the visit function. */
void bst_for_each_range(BST bst, const char *from, const char *to, visit_func_t visit, void *extra);
```

### External Iterator

```c
// The structure for the ADT BST's external iterator
typedef struct bst_iter_t* BSTIterator;

/* Returns an instance of an external iterator for the bst. 

POST:
- if there is not enough memory for the iterator, the function will return NULL.*/
BSTIterator bst_iter_create(BST bst);

/* Returns an instance of an external iterator for the bst. It only iterates through the keys that 
are between `from` and `to`, included.

POST:
- if there is not enough memory for the iterator, the function will return NULL.*/
BSTIterator bst_iter_create_range(BST bst, const char *from, const char *to);

/* Frees the memory where the bst iterator is allocated. */
void bst_iter_destroy(BSTIterator iter);

/* Returns true if there are pairs left to iterate through, false if not */
bool bst_iter_has_next(const BSTIterator iter);

/* Advances the iteration to the next pair.

POST:
- Returns true if the action was successful, false if not. */
bool bst_iter_next(BSTIterator iter);

/* Returns the key of the current pair at the iteration. 

POST:
- If there are no elements left to iterate through, a NULL pointer will be returned.
- The key returned should not be modified nor have its memory freed. */
const char *bst_iter_get_current(const BSTIterator iter);
```
