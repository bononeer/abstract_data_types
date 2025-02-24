# ADT Queue

A simple data structure that follows the FIFO principle - First In, First Out.

## Operations

```c
typedef struct queue_t* Queue;

/* Returns an instance of an empty queue. 

PRE:
- elem_destroy is a pointer to a function that destroys the elements stored in the queue.
If NULL is given, it will not free the memory of the elements. 

POST:
- if there is not enough memory for the queue, the function will return NULL. */
Queue queue_create(void (*elem_destroy)(void* elem));

/* Frees the memory where the queue is allocated */
void queue_destroy(Queue queue);

/* Add `elem`'s value at the end of the queue. 

POST:
- Returns true if the item was successfully added to the queue, and false if there was an 
issue with the operation. */
bool queue_enqueue(Queue queue, void* elem);

/* Delete and return the value of the element at the front of the queue.

POST:
- If the queue is empty, a NULL pointer will be returned.
- If the memory was allocated previously, the returned element should be freed when not needed anymore.
- A void pointer will be returned, the user should cast it to the right type. */
void* queue_dequeue(Queue queue);

/* Return the value of the element at the front of the queue.

POST:
- If the queue is empty, a NULL pointer will be returned.
- A void pointer will be returned, the user should cast it to the right type.*/
void* queue_front(const Queue queue);

/* Returns true if the queue is empty; otherwise, it returns false. */
bool queue_is_empty(const Queue queue);
```
