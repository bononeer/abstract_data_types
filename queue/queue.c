#include <stdlib.h>
#include <string.h>
#include "queue.h"

/******************** structure definition ********************/ 

typedef void (*elem_destroy)(void*);
typedef struct _node node_t;

struct _node {
    void* value;
    node_t* next;
};

struct queue_t {
    node_t* first;
    node_t* last;
    elem_destroy destroy;
};

/******************** static functions declarations ********************/ 

// Return a node with the given value and a null next node.
static node_t* node_create(void* value);
// Frees the memory of a given node.
static void node_destroy(node_t* node, void elem_destroy(void* elem));

/******************** Queue operations definitions ********************/ 

Queue queue_create(void) {
    Queue queue = (Queue)malloc(sizeof(struct queue_t));
    if (queue == NULL) return NULL;

    queue->first = NULL;
    queue->last = NULL;

    return queue;
}

void queue_destroy(Queue queue, void (*elem_destroy)(void* elem)) {
    node_t *current = queue->first;
    if (elem_destroy == NULL) elem_destroy = free;
    while (queue->first != NULL) {
        current = queue->first;
        queue->first = queue->first->next;
        node_destroy(current, elem_destroy);
    }
    free(queue);
}

bool queue_enqueue(Queue queue, void* elem) {
    node_t* new_node = node_create(elem);
    if (new_node == NULL) return false;

    if (queue->last != NULL) queue->last->next = new_node;
    queue->last = new_node;
    if (queue->first == NULL) queue->first = new_node;

    return true;
}

void* queue_dequeue(Queue queue) {
    if (queue_is_empty(queue)) return NULL;
    
    node_t* first = queue->first;
    void* deleted = first->value;

    queue->first = first->next;
    if (first == queue->last) queue->last = NULL;

    free(first);

    return deleted;
}

void* queue_front(const Queue queue) {
    if (queue_is_empty(queue)) return NULL;

    return queue->first->value;
}

bool queue_is_empty(const Queue queue) {
    return queue->first == NULL && queue->last == NULL;
}

/******************** static functions definitions ********************/

static node_t* node_create(void* value) {
    node_t* node = (node_t*)malloc(sizeof(struct _node));
    if (node == NULL) return NULL;

    node->next = NULL;
    node->value = malloc(sizeof(void*));
    if (node->value == NULL) {
        free(node);
        return NULL;
    }
    memcpy(node->value, value, sizeof(void*));

    return node;
}

static void node_destroy(node_t* node, void elem_destroy(void* elem)) {
    elem_destroy(node->value);
    free(node);
}