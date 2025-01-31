# ADT Queue

A simple data structure that follows the FIFO principle - First In, First Out.

## Opreations

```c
typedef struct queue_t* Queue;

/* Returns an instance of an empty queue. 

POST:
- if there is not enough memory for the queue, the function will return NULL. */
Queue queue_create(void);

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
- The returned pointer should be freed when not needed anymore.
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

## Compiling

To compile your code run the following line, where `source_file` is the `.c` that needs `queue.h`.

    gcc -o <output_file> <source_file> queue.c
