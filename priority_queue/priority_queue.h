#ifndef _PRIORITY_QUEUE_H
#define _PRIORITY_QUEUE_H

#include <stdbool.h>
#include <stddef.h>

/******************** PriorityQueue structure definition ********************/ 

/* A compare function that returns the following in different cases:
    - `< 0` if the first element is lesser than the second one. 
    - `== 0` if both elements are equal.
    - `> 0` if the first element is greater than the second one. */
typedef int (*cmp_func_t)(void*, void*);
// A function that defines how to destroy a given generic element.
typedef void (*destroy_func_t)(void*);
/* A data structure that stores elements by a defined priority. The ones with the highest 
priority are the first one to be dequeued from the PriorityQueue. */
typedef struct heap_t *PriorityQueue;

/******************** PriorityQueue operations declarations ********************/

/* Returns an instance of an empty priority queue. 

PRE:
- `cmp` function decides the priority between the elements in the priority queue.
- `elem_destroy` is a pointer to a `destroy_func_t` that defines how to free the memory
of the elements stored in the Stack. If NULL is given, it will not free the memory of 
the elements. 

POST:
- if there is not enough memory for the priority queue, the function will return NULL. */
PriorityQueue p_queue_create(cmp_func_t cmp, destroy_func_t elem_destroy);

/* Returns an instance of an priority queue initiated with the elements from `arr`. 

PRE:
- `arr_len` is the amount of elements inside `arr`
- `cmp` function decides the priority between the elements in the priority queue.
- `elem_destroy` is a pointer to a function that destroys the elements stored in the 
priority queue. If NULL is given, it will destroy the elements with the `free` function 
from stdlib. 

POST:
- Creates a priority queue with elements in O(n) time complexity.
- if there is not enough memory for the priority queue, the function will return NULL. */
PriorityQueue p_queue_create_array(void *arr[], size_t arr_len, cmp_func_t cmp,  destroy_func_t elem_destroy);

/* Frees the memory where the priority queue is allocated. */
void p_queue_destroy(PriorityQueue p_queue);

/* Returns true if the priority queue is empty; otherwise, it returns false. */
bool p_queue_is_empty(const PriorityQueue p_queue);

/* Returns the amount of elements enqueued in `p_queue`. */
size_t p_queue_size(const PriorityQueue p_queue);

/* Add elem to the priority queue. 

POST:
- Returns true if the item was successfully added to the queue, and false if there was 
an issue with the operation. */
bool p_queue_enqueue(PriorityQueue p_queue, void *elem);

/* Return the element with the current maximum priority inside the priority queue.

POST:
- If the priority queue is empty, a NULL pointer will be returned. */
void *p_queue_max(const PriorityQueue p_queue);

/* Delete and return the element with the current maximum priority inside the priority 
queue.

POST:
- If the priority queue is empty, a NULL pointer will be returned.
- If the memory was allocated previously, the returned element should be freed when not 
needed anymore. */
void *p_queue_dequeue(PriorityQueue p_queue);

#endif // _PRIORITY_QUEUE_H