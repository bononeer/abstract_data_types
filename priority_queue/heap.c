#include <stdlib.h>
#include "priority_queue.h"

#define INITIAL_CAPACITY 7
#define VARIATION_CAPACITY 2

/******************** structure definition ********************/ 

struct heap_t {
    void **data;
    size_t size;
    size_t capacity;
    cmp_func_t cmp;
    elem_destroy_t destroy;
};

/******************** static functions declarations ********************/ 

static PriorityQueue heap_create(size_t initial_capacity, size_t initial_size, cmp_func_t cmp, elem_destroy_t elem_destroy);
static bool heap_resize(PriorityQueue heap, size_t new_capacity);
static void up_heap(void *arr[], cmp_func_t cmp, size_t index);
static void down_heap(void *arr[], cmp_func_t cmp, size_t index, size_t length);
static void heapify(void *arr[], cmp_func_t cmp, size_t length);
static size_t father_index(size_t index);
static size_t left_child_index(size_t index);
static void copy(void *dst[], void *src[], size_t length);
static void swap(void **ptr1, void **ptr2);

/******************** PriorityQueue operations definitions ********************/ 

PriorityQueue p_queue_create(cmp_func_t cmp,  elem_destroy_t elem_destroy) {
    return heap_create(INITIAL_CAPACITY, 0, cmp, elem_destroy);
}

PriorityQueue p_queue_create_array(void *arr[], size_t arr_len, cmp_func_t cmp,  elem_destroy_t elem_destroy) {
    PriorityQueue heap = heap_create(INITIAL_CAPACITY >= arr_len ? INITIAL_CAPACITY : arr_len, arr_len, cmp, elem_destroy);
    if (heap == NULL) return NULL;

    copy(heap->data, arr, heap->size);
    heapify(heap->data, heap->cmp, heap->size);

    return heap;
}

void p_queue_destroy(PriorityQueue heap) {
    if (heap->destroy != NULL) for (size_t i = 0 ; i < heap->size ; i++) (heap->destroy)(heap->data[i]);
    free(heap->data);
    free(heap);
}

bool p_queue_enqueue(PriorityQueue heap, void *elem) {
    if (heap->size == heap->capacity) if (!heap_resize(heap, heap->capacity * VARIATION_CAPACITY)) return false;

    heap->data[heap->size] = elem;
    up_heap(heap->data, heap->cmp, heap->size);
    heap->size++;

    return true;
}

void* p_queue_dequeue(PriorityQueue heap) {
    if (p_queue_is_empty(heap)) return NULL;
    if (heap->size * 2 * VARIATION_CAPACITY <= heap->capacity && heap->capacity >= INITIAL_CAPACITY * VARIATION_CAPACITY) if (!heap_resize(heap, heap->capacity / VARIATION_CAPACITY)) return NULL;

    void *deleted = heap->data[0];
    heap->size--;

    swap(&(heap->data)[0], &(heap->data)[heap->size]);
    down_heap(heap->data, heap->cmp, 0, heap->size);
    
    return deleted;
}

void* p_queue_max(const PriorityQueue heap) {
    if (p_queue_is_empty(heap)) return NULL;

    return heap->data[0];
}

bool p_queue_is_empty(const PriorityQueue heap) {
    return heap->size == 0;
}

size_t p_queue_size(const PriorityQueue heap) {
    return heap->size;
}

/******************** static functions definitions ********************/

static PriorityQueue heap_create(size_t initial_capacity, size_t initial_size, cmp_func_t cmp, elem_destroy_t elem_destroy) {
    PriorityQueue heap = (PriorityQueue)malloc(sizeof(struct heap_t));
    if (heap == NULL) return NULL;

    heap->size = initial_size;
    heap->capacity = initial_capacity;
    heap->cmp = cmp;
    heap->destroy = elem_destroy;

    heap->data = (void**)malloc(heap->capacity * sizeof(void*));
    if (heap->data == NULL) {
        free(heap);
        return NULL;
    }

    return heap;
}

static bool heap_resize(PriorityQueue heap, size_t new_capacity) {
    void **data_temp = (void**)realloc(heap->data, new_capacity * sizeof(void*));
    if (data_temp == NULL) return false;

    heap->data = data_temp;
    heap->capacity = new_capacity;

    return true;
}

static void up_heap(void *arr[], cmp_func_t cmp, size_t index) {
    if (index == 0) return;

    size_t father = father_index(index);
    if (cmp(arr[index], arr[father]) <= 0) return;

    swap(&arr[index], &arr[father]);
    up_heap(arr, cmp, father);
}

static void down_heap(void *arr[], cmp_func_t cmp, size_t index, size_t length) {
    size_t left_child = left_child_index(index);
    if (left_child >= length) return;

    size_t right_child = left_child + 1 < length ? left_child + 1 : left_child;

    if (cmp(arr[index], arr[left_child]) >= 0 && (left_child == right_child || cmp(arr[index], arr[right_child]) >= 0)) return;
    size_t max_child = cmp(arr[left_child], arr[right_child]) >= 0 ? left_child : right_child;

    swap(&arr[index], &arr[max_child]);
    down_heap(arr, cmp, max_child, length);
}

static void heapify(void *arr[], cmp_func_t cmp, size_t length) {
    for (size_t i = length - 1 ; i != -1 ; i--) down_heap(arr, cmp, i, length);
}

static size_t father_index(size_t index) {
    return (size_t)((index - 1) / 2);
}

static size_t left_child_index(size_t index) {
    return index * 2 + 1;
}

static void copy(void *dst[], void *src[], size_t length) {
    for (size_t i = 0 ; i < length ; i++) dst[i] = src[i];
}

static void swap(void **ptr1, void **ptr2) {
    void *aux = NULL;
    aux = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = aux;
}