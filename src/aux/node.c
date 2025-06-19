#include "node.h"
#include "aux.h"

/* ############################### Operations ############################### */

node_t *node_create(void *value) {
    node_t *node = (node_t *) malloc(sizeof(struct node));
    if (err_no_mem(node)) return NULL;
    
    node->next = NULL;
    node->value = value;

    return node;
}

void node_destroy(node_t *node, destroy_func_t elem_destroy) {
    if (err_null_ptr(node)) return;

    if (elem_destroy != NULL) elem_destroy(node->value);
    free(node);
}