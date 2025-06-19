#include "queue_aux.h"

/* ############################### Structure ################################ */

struct linked_queue {
    node_t *first;
    node_t *last;
    destroy_func_t destroy;
};

/* ############################### Operations ############################### */

queue_t *queue_create(destroy_func_t elem_destroy) {
    queue_t *queue = (queue_t *) malloc(sizeof(struct linked_queue));
    if (err_null_ptr(queue)) return NULL;

    queue->first = NULL;
    queue->last = NULL;
    queue->destroy = elem_destroy;
    
    return queue;
}

void queue_destroy(void *ptr) {
    if (err_null_ptr(ptr)) return;

    queue_t *queue = (queue_t *) ptr;

    node_t *current = queue->first;
    while (current != NULL) {
        node_t *next = current->next;
        node_destroy(current, queue->destroy);
        current = next;
    }

    free(queue);
}

bool queue_is_empty(queue_t *queue) {
    return !err_null_ptr((queue_t *) queue) && queue->first == NULL && queue->last == NULL;
}

void queue_enqueue(queue_t *queue, void *elem) {
    if (err_null_ptr(queue)) return;
    
    node_t *new_node = node_create(elem);
    if (!new_node) return;

    if (queue->last) queue->last->next = new_node;
    queue->last = new_node;
    if (!queue->first) queue->first = new_node;
}

void *queue_dequeue(queue_t *queue) {
    if (err_empty_queue(queue)) return NULL;

    node_t *first = queue->first;
    void *removed = first->value;

    queue->first = first->next;
    if (first == queue->last) queue->last = NULL;

    free(first);

    return removed;
}

void *queue_front(queue_t *queue) {
    if (err_empty_queue(queue)) return NULL;

    return queue->first->value;
}

/* ############################# Aux Operations ############################# */

bool err_empty_queue(queue_t *queue) {
    if (err_null_ptr(queue)) return true;
    if (!queue_is_empty(queue)) return false;

    errno = ENODATA;
    return true;
}
