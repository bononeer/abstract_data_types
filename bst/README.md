# ADT BST - Binary Search Tree

A data structure that works as a Sorted Map, it stores key-value pairs "in order". The order is decided with a given `cmp_func` in the bst creator. The operations to put, get, check if the key is contained or remove are executed in logarithmic time complexity.

While iterating through the pairs stored in the BST, regardless of the iterator used, the elements will be in order, this means that the key of the current pair is greater than the one just seen and lesser than the one that is next.

## Struct

```c
// A data structure that stores `key-value` pairs with a defined order.
typedef struct bst_t *BST;
// The external iterator for the BST
typedef struct bst_iter_t *BSTIterator;
```

## Operations

### BST

```c
/* Returns an instance of an empty BST. 

PRE:
- `cmp` function decides the order in which the keys will be sorted inside the BST.
- `value_destroy` is a pointer to a `destroy_func_t` that defines how to free the memory
of the value of the pairs stored in the BST. If NULL is given, it will not free the memory 
of the values. 

POST:
- If cmp is NULL, the function returns NULL.
- if there is not enough memory for the BST, the function will return NULL. */
BST bst_create(cmp_func_t cmp, destroy_func_t value_destroy);

/* Frees the memory where the BST is allocated. */
void bst_destroy(BST bst);

/* Returns the amount of pairs stored in the BST. */
size_t bst_size(BST bst);

/* If the key is not stored in the BST, adds the `key-value` pair to the BST; otherwise, 
updates the value of the pair.

POST:
- Returns true if the item was successfully added to the BST, and false if there was an 
issue with the operation. */
bool bst_put(BST bst, char *key, void *value);

/* Returns true if the key is stored in the BST, false if not. */
bool bst_contains(BST bst, const char *key);

/* Return the value of the pair with the given key.

POST:
- If the key is not stored in the BST, the function returns NULL. */
void *bst_get(BST bst, const char *key);

/* Remove and return the value of the pair with the given key. 

POST:
- If the key is not stored in the BST, the function returns NULL.
- If the memory was allocated previously, the returned element should be freed when not 
needed anymore. */
void *bst_remove(BST bst, char *key);

/* Iterates through the pairs of the BST in order according to the cmp function, applying 
the visit function to each one. If `visit(key, value, ...)` return false, the iteration 
stops. 

PRE:
- `extra` is the extra parameter that is given to the visit function. */
void bst_for_each(BST bst, visit_func_t visit, void *extra);

/* Iterates through the pairs of the BST in order according to the cmp function, applying the visit 
function to each one. If `visit(key, value, ...)` return false, the iteration stops. It only iterates
through the keys that are between `from` and `to`, included.

PRE:
- `from` and `to` are two keys contained in the BST. If `from` is NULL, it iterates from the start. 
If `to` is NULL, it iterates until the end. 
- `extra` is the extra parameter that is given to the visit function. */
void bst_for_each_range(BST bst, const char *from, const char *to, visit_func_t visit, void *extra);
```

### External Iterator

```c
/* Returns an instance of an external iterator for the BST. 

POST:
- if there is not enough memory for the iterator, the function will return NULL.*/
BSTIterator bst_iter_create(BST bst);

/* Returns an instance of an external iterator for the BST. It only iterates through the keys that 
are between `from` and `to`, included.

POST:
- if there is not enough memory for the iterator, the function will return NULL. */
BSTIterator bst_iter_range_create(BST bst, const char *from, const char *to);

/* Frees the memory where the BST iterator is allocated. */
void bst_iter_destroy(BSTIterator iter);

/* Returns true if there are pairs left to iterate through, false if not. */
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
