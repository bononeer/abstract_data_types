#ifndef _QUEUE_H
#define _QUEUE_H

#include "adt/adtcomm.h"

/* ############################### Structure ################################ */

/**
 * @brief Data structure that follows the FIFO principle.
 */
typedef struct linked_queue queue_t;

/* ############################### Operations ############################### */

/**
 * @brief Initialize a new empty Queue.
 * 
 * @param elem_destroy Describes the way to destroy the elements stored when
 * `queue_destroy` is used with this Queue.
 * @return Pointer to the new Queue.
 * 
 * @retval `NULL` on error; check `errno` for:
 * @retval - ENOMEM 
 */
queue_t *queue_create(destroy_func_t elem_destroy);

/**
 * @brief Free the Queue's memory and all the elements stored inside.
 * 
 * @retval check `errno` for:
 * @retval - EINVAL
 */
void queue_destroy(void *queue);

/**
 * @brief Check if the Queue is empty.
 * 
 * @retval `true` if the Queue is empty 
 * @retval `false` otherwise; check `errno` for:
 * @retval - EINVAL
 */
bool queue_is_empty(queue_t *queue);

/**
 * @brief Add a new element to the back ot the Queue.
 * 
 * @param elem The new element to be stored.
 * 
 * @retval check `errno` for:
 * @retval - EINVAL
 * @retval - ENOMEM 
 */
void queue_enqueue(queue_t *queue, void *elem);

/**
 * @brief Remove the first element of the Queue.
 * 
 * @return `void *` The removed element.
 * 
 * @retval `NULL` on error; check `errno` for:
 * @retval - EINVAL
 * @retval - ENODATA 
 */
void *queue_dequeue(queue_t *queue);

/**
 * @brief Get the first element of the Queue.
 * 
 * @return `void *` The first element
 * 
 * @retval `NULL` on error; check `errno` for:
 * @retval - EINVAL
 * @retval - ENODATA
 */
void *queue_front(queue_t *queue);

#endif // _QUEUE_H
