#ifndef _LIST_H
#define _LIST_H

#include <stdbool.h>
#include <stddef.h>

/******************** List structures declarations ********************/ 

// A function that defines how to destroy a given generic element.
typedef void (*destroy_func_t)(void*);
/* A function for the internal iterators which returns a boolean value to decide if the 
iteration continues or not. It receives an `extra` parameter that can be NULL. */
typedef bool (*visit_func_t)(void*, void* extra);
// A data structure that stores elements in a linear order
typedef struct list_t *List;
// The external iterator for the List
typedef struct list_iter_t *ListIterator;

/******************** List operations declarations ********************/

/* Returns an instance of an empty List. 

PRE:
- `elem_destroy` is a pointer to a `destroy_func_t` that defines how to free the memory
of the elements stored in the List. If NULL is given, it will not free the memory of 
the elements. 

POST:
- if there is not enough memory for the List, the function will return NULL. */
List list_create(destroy_func_t elem_destroy);

/* Frees the memory where the List is allocated */
void list_destroy(List list);

/* Returns true if the List is empty; otherwise, it returns false. */
bool list_is_empty(const List list);

/* Returns the amount of elements currently stored in the List. */
size_t list_length(const List list);

/* Add an element at the start of the List. 

POST:
- Returns true if the item was successfully added to the List, and false if there was an 
issue with the operation. */
bool list_insert_first(List list, void *elem);

/* Add an element at the end of the List. 

POST:
- Returns true if the item was successfully added to the List, and false if there was an 
issue with the operation. */
bool list_insert_last(List list, void *elem);

/* Return the element at the start of the List.

POST:
- If the List is empty, a NULL pointer will be returned. */
void *list_get_first(const List list);

/* Return the element at the end of the List.

POST:
- If the List is empty, a NULL pointer will be returned. */
void *list_get_last(const List list);

/* Remove the element at the start of the List and return it. 

POST:
- If the List is empty, a NULL pointer will be returned.
- If the memory was allocated previously, the memory of the returned element should be 
freed when not needed anymore. */
void *list_delete_first(List list);

/* The internal iterator of the List. Iterates through the elements of the List, applying 
the visit function to each one. If `visit(elem, ...)` return false, the iteration stops. 

PRE:
- `visit` is a function that returns a boolean value, it receives the current element in
the iteration and an extra value that could be NULL.
- `extra` is an extra value that could be NULL, if it is not, it must be passed to the
`visit` function. */
void list_for_each(List list, visit_func_t visit, void *extra);

/******************** List Iterator operations declarations ********************/

/* Returns an instance of an external iterator for the List. 

POST:
- if there is not enough memory for the List Iterator, the function will return NULL. */
ListIterator list_iter_create(List list);

/* Frees the memory where the List Iterator is allocated. */
void list_iter_destroy(ListIterator iter);

/* Returns true if there are elements left to iterate through, false if not */
bool list_iter_has_next(const ListIterator iter);

/* Advances the iteration to the next element.

POST:
- Returns true if the action was successful, false if not. */
bool list_iter_next(ListIterator iter);

/* Returns the value of the current element at the iteration. 

POST:
- If there are no elements left to iterate through, a NULL pointer will be returned. */
void *list_iter_get_current(const ListIterator iter);

/* Inserts an element in the List between the last and current elements in the iteration. 

POST:
- The new element will be the current element in the iteration.
- Returns true if the item was successfully added to the list, and false if there was an 
issue with the operation. */
bool list_iter_insert(ListIterator iter, void *elem);

/* Remove the current element of the iteration from the List and returns it.

POST:
- The next element of the iteration will be the current element.
- If the List is empty, a NULL pointer will be returned.
- If the memory was allocated previously, the returned element should be freed when not 
needed anymore. */
void *list_iter_delete(ListIterator iter);

#endif // _LIST_H