#include "list_aux.h"

/* ############################### Structures ############################### */

struct linked_list {
    node_t *first;
    node_t *last;
    size_t length;
    destroy_func_t destroy;
};

struct linked_list_iter {
    list_t *list;
    node_t *current;
    node_t *prev;
};

/* ############################### Operations ############################### */

list_t *list_create(destroy_func_t elem_destroy) {
    list_t *list = (list_t *) malloc(sizeof(struct linked_list));
    if (err_no_mem(list)) return NULL;

    list->first = NULL;
    list->last = NULL;
    list->length = 0;
    list->destroy = elem_destroy;

    return list;
}

void list_destroy(void *ptr) {
    if (err_null_ptr(ptr)) return;

    list_t *list = (list_t *) ptr;

    node_t *current = list->first;
    while (current != NULL) {
        node_t *next = current->next;
        node_destroy(current, list->destroy);
        current = next;
    }

    free(list);
}

size_t list_length(list_t *list) {
    return err_null_ptr(list) ? 0 : list->length;
}

void list_insert_first(list_t *list, void *elem) {
    if (err_null_ptr(list)) return;

    node_t *new_node = node_create(elem);
    if (err_no_mem(new_node)) return;

    if (!list->first && !list->last) list->last = new_node;
    else new_node->next = list->first;
    
    list->first = new_node;
    list->length++;
}

void list_insert_last(list_t *list, void *elem) {
    if (err_null_ptr(list)) return;

    node_t *new_node = node_create(elem);
    if (err_no_mem(new_node)) return;

    if (!list->first && !list->last) list->first = new_node;
    else list->last->next = new_node;
    
    list->last = new_node;
    list->length++;
}

void *list_remove_first(list_t *list) {
    if (err_empty_list(list)) return NULL;

    node_t *first = list->first;
    void *removed = first->value;

    list->first = first->next;

    free(first);
    list->length--;

    return removed;
}

void *list_get_first(list_t *list) {
    if (err_empty_list(list)) return NULL;

    return list->first->value;
}

void *list_get_last(list_t *list) {
    if (err_empty_list(list)) return NULL;

    return list->last->value;
}

void list_for_each(list_t *list, smpl_visit_func_t visit, void *extra) {
    node_t *current = list->first;
    while(current != NULL && visit(current->value, extra)) {
        current = current->next;
    }
}

/* ########################## Iterator Operations ########################### */

list_iterator_t *list_iter_create(list_t *list) {
    if (err_null_ptr(list)) return NULL;

    list_iterator_t *iter = (list_iterator_t *) malloc(sizeof(struct linked_list_iter));
    if (err_no_mem(iter)) return NULL;

    iter->list = list;
    iter->current = list->first;
    iter->prev = NULL;

    return iter;
}

void list_iter_destroy(void *iter) {
    if (err_null_ptr(iter)) return;

    free(iter);
}

bool list_iter_has_next(list_iterator_t *iter) {
    return !err_null_ptr(iter) && iter->current != NULL;
}

void list_iter_next(list_iterator_t *iter) {
    if (err_finsihed_list_iter(iter)) return;

    iter->prev = iter->current;
    iter->current = iter->current->next;
}

void *list_iter_get_current(list_iterator_t *iter) {
    if (err_finsihed_list_iter(iter)) return NULL;

    return iter->current->value;
}

void list_iter_insert(list_iterator_t *iter, void *elem) {
    if (err_null_ptr(iter)) return;

    node_t *new_node = node_create(elem);
    if (err_no_mem(new_node)) return;

    new_node->next = iter->current;
    if (!iter->current) iter->list->last = new_node;
    if (!iter->prev) iter->list->first = new_node;
    else iter->prev->next = new_node;

    iter->current = new_node;
    iter->list->length++;
}

void *list_iter_remove(list_iterator_t *iter) {
    if (err_finsihed_list_iter(iter)) return NULL;

    node_t *rem_node = iter->current;
    void *removed = rem_node->value;

    if (iter->prev != NULL) iter->prev->next = rem_node->next;
    free(rem_node);

    iter->list->length--;

    return removed;

    return removed;
}

/* ############################# Aux Operations ############################# */

bool err_empty_list(list_t *list) {
    if (list_length(list) > 0) return false;

    errno = ENODATA;
    return true;
}

bool err_finsihed_list_iter(list_iterator_t *iter) {
    if (list_iter_has_next(iter)) return false;
    
    errno = ENODATA;
    return true;
}