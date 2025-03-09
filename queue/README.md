# ADT Queue

A simple data structure that follows the FIFO principle - First In, First Out.

## Struct

```c
// A data structure that follows the FIFO principle.
typedef struct queue_t *Queue;
```

## Operations

```c
/* Returns an instance of an empty Queue. 

PRE:
- `elem_destroy` is a pointer to a `destroy_func_t` that defines how to free the memory
of the elements stored in the Queue. If NULL is given, it will not free the memory of 
the elements. 

POST:
- if there is not enough memory for the Queue, the function will return NULL. */
Queue queue_create(destroy_func_t elem_destroy);

/* Frees the memory where the Queue is allocated */
void queue_destroy(Queue queue);

/* Returns true if the Queue is empty; otherwise, it returns false. */
bool queue_is_empty(const Queue queue);

/* Adds an element to the end of the Queue.

POST:
- Returns true if the item was successfully added to the Queue, and false if there was an 
issue with the operation. */
bool queue_enqueue(Queue queue, void* elem);

/* Return the element at the front of the Queue. 

POST:
- If the Queue is empty, a NULL pointer will be returned. */
void *queue_front(const Queue queue);

/* Remove the element at the front of the Queue and return it. 

POST:
- If the Queue is empty, a NULL pointer will be returned.
- If the memory was allocated previously, the memory of the returned element should be 
freed when not needed anymore. */
void *queue_dequeue(Queue queue);
```
