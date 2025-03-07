#include <stdlib.h>
#include "queue.h"

/******************** structure definition ********************/ 

typedef struct node node_t;

struct node {
    void *value;
    node_t *next;
};

struct queue_t {
    node_t *first;
    node_t *last;
    destroy_func_t destroy;
};

/******************** static functions declarations ********************/ 

static node_t *node_create(void *value);
static void node_destroy(node_t *node, destroy_func_t elem_destroy);

/******************** Queue operations definitions ********************/ 

Queue queue_create(destroy_func_t elem_destroy) {
    Queue queue = (Queue)malloc(sizeof(struct queue_t));
    if (queue == NULL) return NULL;

    queue->first = NULL;
    queue->last = NULL;
    queue->destroy = elem_destroy;

    return queue;
}

void queue_destroy(Queue queue) {
    if (queue == NULL) return;

    node_t *current = queue->first;
    while (queue->first != NULL) {
        current = queue->first;
        queue->first = queue->first->next;
        node_destroy(current, queue->destroy);
    }
    free(queue);
}

bool queue_is_empty(const Queue queue) {
    return queue != NULL && queue->first == NULL && queue->last == NULL;
}

bool queue_enqueue(Queue queue, void* elem) {
    if (queue == NULL) return false;

    node_t *new_node = node_create(elem);
    if (new_node == NULL) return false;

    if (queue->last != NULL) queue->last->next = new_node;
    queue->last = new_node;
    if (queue->first == NULL) queue->first = new_node;

    return true;
}

void *queue_front(const Queue queue) {
    if (queue == NULL || queue_is_empty(queue)) return NULL;

    return queue->first->value;
}

void *queue_dequeue(Queue queue) {
    if (queue == NULL || queue_is_empty(queue)) return NULL;
    
    node_t *first = queue->first;
    void *deleted = first->value;

    queue->first = first->next;
    if (first == queue->last) queue->last = NULL;

    free(first);

    return deleted;
}

/******************** static functions definitions ********************/

static node_t *node_create(void* value) {
    node_t *node = (node_t*)malloc(sizeof(struct node));
    if (node == NULL) return NULL;

    node->next = NULL;
    node->value = value;

    return node;
}

static void node_destroy(node_t *node, destroy_func_t elem_destroy) {
    if (elem_destroy != NULL) (elem_destroy)(node->value);
    free(node);
}