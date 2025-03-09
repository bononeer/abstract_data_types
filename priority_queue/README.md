# ADT PriorityQueue

A data structure that manages elements with a defined priority. A `cmp_func_t` is needed, whose job is to compare between its elements to decide the order in which they will be out of the priority queue. The ones with the highest priority are the first ones to be dequeued.

For this **Priority Queue** implementation a **Heap** is used. This means that the main operations are executed in logarithmic time compĺexity (enqueue, dequeue). Create an empty priority queue would be constant time complexity, but creating a priority queue from an array is executed in linear time complexity, which means this operations is faster than creating an empty one and enqueueing the elements from the array one by one.

## Struct

```c
/* A data structure that stores elements by a defined priority. The ones with the highest 
priority are the first one to be dequeued from the PriorityQueue. */
typedef struct heap_t *PriorityQueue;
```

## Operations

```c
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
```
