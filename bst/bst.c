#include <stdlib.h>
#include <string.h>
#include "bst.h"
#include "stack.h"

/******************** structure definition ********************/

typedef struct _bst_node bst_node_t;

struct _bst_node {
    char *key;
    void *value;
    bst_node_t* left;
    bst_node_t* right;
};

struct bst_t {
    bst_node_t* root;
    size_t size;
    cmp_func_t cmp;
    value_destroy_t destroy;
};

struct bst_iter_t {
    BST bst;
    char *from;
    char *to;
    Stack remaining;
};

/******************** static functions declarations ********************/ 

static void bst_destroy_helper(bst_node_t *node, value_destroy_t value_destroy);
static bool bst_for_each_helper(bst_node_t *node, visit_func_t visit, void *extra, const char *from, const char *to, cmp_func_t cmp);
static BSTIterator bst_iter_create_helper(BST bst, const char *from, const char* to);
static bst_node_t *node_create(const char *key, void *value);
static void node_destroy(bst_node_t *node, value_destroy_t value_destroy);
static bst_node_t *bst_search(BST bst, const char *key);
static bst_node_t *bst_search_father(BST bst, const char *key);
static bst_node_t *bst_search_father_helper(BST bst, bst_node_t *father, bst_node_t *node, const char *key);
static bst_node_t *get_child(bst_node_t *father, const char *key, cmp_func_t cmp);
static unsigned char direct_children(bst_node_t *node);
static bst_node_t *get_only_child(bst_node_t *node);
static char *find_heir(bst_node_t *node);
static char *strdup(const char *src);
static bool push_node_left_branch(BSTIterator iter, bst_node_t *node);

/******************** BST operations definitions ********************/

BST bst_create(cmp_func_t cmp, value_destroy_t value_destroy) {
    BST bst = (BST)malloc(sizeof(struct bst_t));
    if (bst == NULL) return NULL;

    bst->root = NULL;
    bst->size = 0;
    if (cmp == NULL) return NULL;
    bst->cmp = cmp;
    bst->destroy = value_destroy;

    return bst;
}

void bst_destroy(BST bst) {
    if (bst == NULL) return;

    bst_destroy_helper(bst->root, bst->destroy);
    free(bst);
}

size_t bst_size(BST bst) {
    if (bst == NULL ) return 0;
    return bst->size;
}

bool bst_put(BST bst, char *key, void *value) {
    if (bst == NULL) return false;

    bst_node_t *father = bst_search_father(bst, key);
    bst_node_t *child = get_child(father, key, bst->cmp);

    if (child != NULL) {
        if (bst->destroy != NULL) (bst->destroy)(child->value);
        child->value = value;
        
        return true;
    }

    bst_node_t *new_node = node_create(key, value);
    if (new_node == NULL) return false;

    if (father == NULL) bst->root = new_node;
    else if (bst->cmp(key, father->key) < 0) father->left = new_node;
    else father->right = new_node;

    bst->size++;

    return true;
}

bool bst_contains(BST bst, const char *key) {
    if (bst == NULL) return false;
    bst_node_t *node = bst_search(bst, key);

    return node != NULL;
}

void *bst_get(BST bst, const char *key) {
    if (bst == NULL) return NULL;
    bst_node_t *node = bst_search(bst, key);
    if (node == NULL) return NULL;

    return node->value;
}

void *bst_remove(BST bst, char *key) {
    if (bst == NULL) return NULL;
    bst_node_t *father = bst_search_father(bst, key);
    bst_node_t *node = get_child(father, key, bst->cmp);
    if (node == NULL) return NULL;
    
    bst_node_t **node_dir;
    if (bst->cmp(key, bst->root->key) == 0) node_dir = &bst->root; 
    else if (father->left != NULL && bst->cmp(key, father->left->key) == 0) node_dir = &father->left;
    else if (father->right != NULL) node_dir = &father->right;

    void *deleted = node->value;

    unsigned char children_amount = direct_children(node);
    if (children_amount == 0 || children_amount == 1) {
        free(node->key);
        *node_dir = get_only_child(node);
        free(node);
        bst->size--;
    } else if (children_amount == 2) {
        char *heir_key = find_heir(node->left);
        char *key_temp = strdup(heir_key);
        void *heir_value = bst_remove(bst, heir_key);   // recursive call cannot be 2 children case
        free((*node_dir)->key);
        (*node_dir)->key = key_temp;
        (*node_dir)->value = heir_value;
    }

    return deleted;
}

void bst_for_each(BST bst, visit_func_t visit, void *extra) {
    if (bst == NULL) return;
    bst_for_each_helper(bst->root, visit, extra, NULL, NULL, bst->cmp);
}

void bst_for_each_range(BST bst, const char *from, const char *to, visit_func_t visit, void *extra) {
    if (bst == NULL) return;
    bst_for_each_helper(bst->root, visit, extra, from, to, bst->cmp);
}

/******************** BST Iterator operations definitions ********************/

BSTIterator bst_iter_create(BST bst) {
    if (bst == NULL) return NULL;
    BSTIterator iter = bst_iter_create_helper(bst, NULL, NULL);
    if (iter == NULL) return NULL;
    
    return iter;
}

BSTIterator bst_iter_create_range(BST bst, const char *from, const char *to) {
    if (bst == NULL) return NULL;
    BSTIterator iter = bst_iter_create_helper(bst, from, to);
    if (iter == NULL) return NULL;
    
    return iter;
}

void bst_iter_destroy(BSTIterator iter) {
    if (iter == NULL) return;
    
    stack_destroy(iter->remaining);
    free(iter);
}

bool bst_iter_has_next(BSTIterator iter) {
    return iter != NULL && !stack_is_empty(iter->remaining);
}

bool bst_iter_next(BSTIterator iter) {
    if (!bst_iter_has_next(iter)) return false;

    bst_node_t *current = (bst_node_t*)stack_pop(iter->remaining);
    if (current == NULL) return false;

    return push_node_left_branch(iter, current->right);
}

const char *bst_iter_get_current(BSTIterator iter) {
    if (!bst_iter_has_next(iter)) return NULL;
    bst_node_t *current = (bst_node_t*)stack_top(iter->remaining);
    
    return current->key;
}

/******************** static functions definitions ********************/

static void bst_destroy_helper(bst_node_t *node, value_destroy_t value_destroy) {
    if (node == NULL) return;

    bst_destroy_helper(node->left, value_destroy);
    bst_destroy_helper(node->right, value_destroy);
    node_destroy(node, value_destroy);
}

static bool bst_for_each_helper(bst_node_t *node, visit_func_t visit, void *extra, const char *from, const char *to, cmp_func_t cmp) {
    if (node == NULL) return true;

    bool iterate_left = from == NULL || cmp(from, node->key) < 0;
    if (iterate_left && !bst_for_each_helper(node->left, visit, extra, from, to, cmp)) return false;
    
    bool iterate_current = (from == NULL || cmp(from, node->key) <= 0) && (to == NULL || cmp(to, node->key) >= 0);
    if (iterate_current && !visit(node->key, node->value, extra)) return false;
    
    bool iterate_right = to == NULL || cmp(to, node->key) > 0;
    if (iterate_right && !bst_for_each_helper(node->right, visit, extra, from, to, cmp)) return false;

    return true;
}

static BSTIterator bst_iter_create_helper(BST bst, const char *from, const char* to) {
    BSTIterator iter = (BSTIterator)malloc(sizeof(struct bst_iter_t));
    if (iter == NULL) return NULL;

    iter->bst = bst;
    iter->from = (char*)from;
    iter->to = (char*)to;
    iter->remaining = stack_create(NULL);
    if (iter->remaining == NULL) {
        free(iter);
        return NULL;
    }

    if (!push_node_left_branch(iter, bst->root)) {
        stack_destroy(iter->remaining);
        free(iter);
        return NULL;
    }

    return iter;
}

static bst_node_t *node_create(const char *key, void *value) {
    bst_node_t *node = (bst_node_t*)malloc(sizeof(bst_node_t));
    if (node == NULL) return NULL;

    node->key = strdup(key);
    if (node->key == NULL) {
        free(node);
        return NULL;
    }
    node->value = value;
    node->left = NULL;
    node->right = NULL;

    return node;
}

static void node_destroy(bst_node_t* node, value_destroy_t value_destroy) {
    if (value_destroy != NULL) (value_destroy)(node->value);
    free(node->key);
    free(node);
}

static bst_node_t *bst_search(BST bst, const char* key) {
    bst_node_t *father = bst_search_father(bst, key);

    return get_child(father, key, bst->cmp);
}

static bst_node_t *bst_search_father(BST bst, const char *key) {
    return bst_search_father_helper(bst, bst->root, bst->root, key);
}

static bst_node_t *bst_search_father_helper(BST bst, bst_node_t *father, bst_node_t *node, const char* key) {
    if (node == NULL || bst->cmp(key, node->key) == 0) return father;
    
    if (bst->cmp(key, node->key) < 0) return bst_search_father_helper(bst, node, node->left, key);
    return bst_search_father_helper(bst, node, node->right, key);
}

static bst_node_t *get_child(bst_node_t *father, const char *key, cmp_func_t cmp) {
    if (father == NULL) return NULL;

    if (cmp(key, father->key) == 0) return father;
    if (cmp(key, father->key) < 0) return father->left != NULL ? father->left : NULL;

    return father->right != NULL ? father->right : NULL;
}

static unsigned char direct_children(bst_node_t *node) {
    return (unsigned char)((node->left != NULL) + (node->right != NULL));
}

static bst_node_t *get_only_child(bst_node_t *node) {
    return node->left != NULL ? node->left : node->right;
}

static char *find_heir(bst_node_t *node) {
    return node->right == NULL ? node->key : find_heir(node->right); 
}

static char *strdup(const char *src) {
    char *string = (char*)malloc((strlen(src)+1) * sizeof(char));
    if (string == NULL) return NULL;
    strcpy(string, src);

    return string;
}

static bool push_node_left_branch(BSTIterator iter, bst_node_t *node) {
    if (node == NULL) return true;

    bool reached_left = iter->from != NULL && iter->bst->cmp(node->key, iter->from) < 0;
    bool reached_right = iter->to != NULL && iter->bst->cmp(node->key, iter->to) > 0;
    if (reached_left || reached_right) return true;

    while (iter->from != NULL && iter->bst->cmp(node->key, iter->from) < 0) node = node->right;
    while (iter->to != NULL && iter->bst->cmp(node->key, iter->to) > 0) node = node->left;
    if (!stack_push(iter->remaining, node)) return false;

    return push_node_left_branch(iter, node->left);
}