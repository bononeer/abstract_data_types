#include <stdlib.h>
#include <string.h>
#include "list.h"

/******************** structure definition ********************/ 

typedef struct _node node_t;

struct _node {
    void* value;
    node_t* next;
};

struct list_t {
    node_t* first;
    node_t* last;
    size_t length;
};

struct list_iter_t {
    List list;
    node_t* current;
    node_t* past;
};

/******************** static functions declarations ********************/ 

// Return a node with the given value and a null next node.
static node_t* node_create(void* value);
// Frees the memory of a given node.
static void node_destroy(node_t* node);

/******************** List operations definitions ********************/

List list_create(void) {
    List list = (List)malloc(sizeof(struct list_t));
    if (list == NULL) return NULL;

    list->first = NULL;
    list->last = NULL;
    list->length = 0;

    return list;
}

void list_destroy(List list) {
    node_t *current = list->first;
    while (list->first != NULL) {
        current = list->first;
        list->first = list->first->next;
        node_destroy(current);
    }
    free(list);
}

bool list_is_empty(const List list) {
    return list->first == NULL && list->last == NULL && list->length == 0;
}

size_t list_length(const List list) {
    return list->length;
}

bool list_insert_first(List list, void* elem) {
    node_t* new_node = node_create(elem);
    if (new_node == NULL) return false;

    if (list_is_empty(list)) list->last = new_node;
    else new_node->next = list->first;

    list->first = new_node;
    list->length++;

    return true;
}

bool list_insert_last(List list, void* elem) {
    node_t* new_node = node_create(elem);
    if (new_node == NULL) return false;

    if (list_is_empty(list)) list->first = new_node;
    else list->last->next = new_node;

    list->last = new_node;
    list->length++;

    return true;
}

void* list_delete_first(List list) {
    if (list_is_empty(list)) return NULL;
    
    node_t* first = list->first;
    void* deleted = first->value;
    
    list->first = first->next;
    list->length--;
    if (list->length == 0) list->last = NULL;

    free(first);

    return deleted;
}

void* list_get_first(const List list) {
    if (list_is_empty(list)) return NULL;

    return list->first->value;
}

void* list_get_last(const List list) {
    if (list_is_empty(list)) return NULL;

    return list->last->value;
}

void list_for_each(List list, bool visit(void* elem, void* extra), void* extra) {
    node_t *current = list->first;

    while(current != NULL && visit(current->value, extra)) {
        current = current->next;
    }
}

/******************** List Iterator operations definitions ********************/

ListIterator list_iter_create(List list) {
    ListIterator iter = (ListIterator)malloc(sizeof(struct list_iter_t));
    if (iter == NULL) return NULL;

    iter->list = list;
    iter->current = list->first;
    iter->past = NULL;

    return iter;
}

void list_iter_destroy(ListIterator iter) {
    free(iter);
}

bool list_iter_has_next(const ListIterator iter) {
    return iter->current != NULL;
}

bool list_iter_next(ListIterator iter) {
    if (!list_iter_has_next(iter)) return false;

    iter->current = iter->current->next;
    if (iter->past == NULL) iter->past = iter->list->first;
    else iter->past = iter->past->next;

    return true;
}

void* list_iter_get_current(const ListIterator iter) {
    if (!list_iter_has_next(iter)) return NULL;

    return iter->current->value;
}

bool list_iter_insert(ListIterator iter, void* elem) {
    node_t* new_node = node_create(elem);
    if (new_node == NULL) return false;

    new_node->next = iter->current;
    if (iter->current == NULL) iter->list->last = new_node;
    if (iter->past == NULL) iter->list->first = new_node;
    else iter->past->next = new_node;
    
    iter->current = new_node;
    iter->list->length++;

    return true;
}

void* list_iter_delete(ListIterator iter) {
    if (!list_iter_has_next(iter)) return NULL;

    node_t *current = iter->current, *first = iter->list->first, *last = iter->list->last;
    void* deleted = iter->current->value;

    if (first == current) iter->list->first = first->next;
    if (last == current) iter->list->last = iter->past;

    iter->current = current->next;
    iter->list->length--;

    free(current);

    return deleted;
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

static void node_destroy(node_t* node) {
    free(node->value);
    free(node);
}