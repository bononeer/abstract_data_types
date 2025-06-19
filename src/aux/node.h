#ifndef _NODE_H
#define _NODE_H

#include "adt/adtcomm.h"

/* ############################### Structure ################################ */

typedef struct node {
    struct node *next;
    void *value;
} node_t;

/* ############################### Operations ############################### */

node_t *node_create(void *value);

void node_destroy(node_t *node, destroy_func_t elem_destroy);

#endif // _NODE_H