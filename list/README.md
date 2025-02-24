# ADT List

A collection of elements organized in a linear order.
Inlcudes an internal iterator and an external iterator.

## Operations

### List

```c
typedef struct list_t* List;

/* Returns an instance of an empty list. 

PRE:
- elem_destroy is a pointer to a function that destroys the elements stored in the list.
If NULL is given, it will not free the memory of the elements. 

POST:
- if there is not enough memory for the list, the function will return NULL. */
List list_create(void (*elem_destroy)(void* elem));

/* Frees the memory where the list is allocated */
void list_destroy(List list);

/* Returns true if the list is empty; otherwise, it returns false. */
bool list_is_empty(const List list);

/* Returns the amount of elements currently stored in the list. */
size_t list_length(const List list);

/* Add `elem`'s value at the start of the list. 

POST:
- Returns true if the item was successfully added to the list, and false if there was an 
issue with the operation. */
bool list_insert_first(List list, void* elem);

/* Add `elem`'s value at the end of the list. 

POST:
- Returns true if the item was successfully added to the list, and false if there was an 
issue with the operation. */
bool list_insert_last(List list, void* elem);

/* Delete and return the value of the element at the start of the list.

POST:
- If the list is empty, a NULL pointer will be returned.
- If the memory was allocated previously, the returned element should be freed when not needed anymore.
- A void pointer will be returned, the user should cast it to the right type. */
void* list_delete_first(List list);

/* Return the value of the element at the start of the list.

POST:
- If the list is empty, a NULL pointer will be returned.
- A void pointer will be returned, the user should cast it to the right type.*/
void* list_get_first(const List list);

/* Return the value of the element at the end of the list.

POST:
- If the list is empty, a NULL pointer will be returned.
- A void pointer will be returned, the user should cast it to the right type.*/
void* list_get_last(const List list);

/* The internal iterator of the list. Iterates through the elements of the list, applying 
the visit function to each one. If `visit(elem, ...)` return false, the iteration stops. 

PRE:
- `visit` is a function that returns a boolean value, it receives the current element in
the iteration and an extra value that could be NULL.
- `extra` is an extra value that could be NULL, if it is not, it must be passed to the
`visit` function. */
void list_for_each(List list, bool visit(void* elem, void* extra), void* extra);
```

### External Iterator

```c
typedef struct list_iter_t* ListIterator;

/* Returns an instance of an external iterator for the list. 

POST:
- if there is not enough memory for the iterator, the function will return NULL.*/
ListIterator list_iter_create(List list);

/* Frees the memory where the list iterator is allocated. */
void list_iter_destroy(ListIterator iter);

/* Returns true if there are elements left to iterate though, false if not */
bool list_iter_has_next(const ListIterator iter);

/* Advances the iteration to the next element.

POST:
- Returns true if the action was successful, false if not. */
bool list_iter_next(ListIterator iter);

/* Returns the value of the current element at the iteration. 

POST:
- If there are no elements left to iterate through, a NULL pointer will be returned.
- A void pointer will be returned, the user should cast it to the right type. */
void* list_iter_get_current(const ListIterator iter);

/* Inserts an element in the list between the last and current elements in the iteration. 

POST:
- The new element will be the current element in the iteration.
- Returns true if the item was successfully added to the list, and false if there was an 
issue with the operation. */
bool list_iter_insert(ListIterator iter, void* elem);

/* Delete and return the value of the current element in the iteration.

POST:
- The next element will be the current element in the iteration.
- If the list is empty, a NULL pointer will be returned.
- If the memory was allocated previously, the returned element should be freed when not needed anymore.
- A void pointer will be returned, the user should cast it to the right type. */
void* list_iter_delete(ListIterator iter);
```
