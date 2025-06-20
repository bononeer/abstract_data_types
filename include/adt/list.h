#ifndef _LIST_H
#define _LIST_H

#include "adt/adtcomm.h"

/* ############################### Structures ############################### */

/**
 * @brief Data structure that lets you add and delete elements while you iterate
 * through them.
 */
typedef struct linked_list list_t;

/**
 * @brief The external iterator for the List.
 */
typedef struct linked_list_iter list_iterator_t;

/* ############################### Operations ############################### */

/**
 * @brief Initialize a new empty List.
 * 
 * @param elem_destroy Describes the way to destroy the elements stored when
 * `list_destroy` is used with this List.
 * @return Pointer to the new List.
 * 
 * @retval `NULL` on error; check `errno` for:
 * @retval - ENOMEM
 */
list_t *list_create(destroy_func_t elem_destroy);

/**
 * @brief Free the List's memory and all the elements stored inside.
 * 
 * @retval check `errno` for:
 * @retval - EINVAL
 */
void list_destroy(void *list);

/**
 * @brief Get the amount of elements inside the List.
 * 
 * @return `size_t` Length of the List.
 * @retval `0` if error; check `errno` for:
 * @retval - EINVAL
 */
size_t list_length(list_t *list);

/**
 * @brief Add a new element at the start of the List.
 * 
 * @param elem The new element to be stored.
 * 
 * @retval check `errno` for:
 * @retval - EINVAL
 * @retval - ENOMEM 
 */
void list_insert_first(list_t *list, void *elem);

/**
 * @brief Add a new element at the end of the List.
 * 
 * @param elem The new element to be stored.
 * 
 * @retval check `errno` for:
 * @retval - EINVAL
 * @retval - ENOMEM 
 */
void list_insert_last(list_t *list, void *elem);

/**
 * @brief Remove the first element of the List.
 * 
 * @return `void *` The removed element.
 * 
 * @retval `NULL` on error; check `errno` for:
 * @retval - EINVAL
 * @retval - ENODATA
 */
void *list_remove_first(list_t *list);

/**
 * @brief Get the first element of the List.
 * 
 * @return `void *` The first element.
 * 
 * @retval `NULL` on error; check `errno` for:
 * @retval - EINVAL
 * @retval - ENODATA
 */
void *list_get_first(list_t *list);

/**
 * @brief Get the last element of the List.
 * 
 * @return `void *` The last element.
 * 
 * @retval `NULL` on error; check `errno` for:
 * @retval - EINVAL
 * @retval - ENODATA
 */
void *list_get_last(list_t *list);

/**
 * @brief The internal iterator for the List.
 * 
 * @param visit Function that will stop the iteration when the current element is
 * evaluated as `false`.
 * @param extra An extra element that may be given to `visit` to help.
 *
 * @note The iterarion will also stop if every single element was evaluated as 
 * `true`.
 * 
 * @retval check `errno` for:
 * @retval - EINVAL
 */
void list_for_each(list_t *list, smpl_visit_func_t visit, void *extra);

/* ########################## Iterator Operations ########################### */

/**
 * @brief Initialize an external iterator for a List. 
 * 
 * @return Pointer to the iterator for the given List.
 * 
 * @retval `NULL` on error; check `errno` for:
 * @retval - EINVAL 
 * @retval - ENOMEM
 */
list_iterator_t *list_iter_create(list_t *list);

/**
 * @brief Free the Iterator's memory.
 * 
 * @retval check `errno` for:
 * @retval - EINVAL
 * 
 * @note it doesn't free the memory of any element that was or wasn't iterated
 * through.
 */
void list_iter_destroy(void *iter);

/**
 * @brief Check if the iterator has finished or not.
 * 
 * @retval `true` If there are.
 * @retval `false` otherwise; check `errno` for:
 * @retval - EINVAL
 */
bool list_iter_has_next(list_iterator_t *iter);

/**
 * @brief Step over to the next element in the iteration.
 *
 * @retval check `errno` for:
 * @retval - EINVAL 
 * @retval - ENODATA 
 */
void list_iter_next(list_iterator_t *iter);

/**
 * @brief Get the element at the current position of the iteration.
 * 
 * @return `void *` The current element.
 * 
 * @retval `NULL` on error; check `errno` for:
 * @retval - EINVAL
 * @retval - ENODATA 
 */
void *list_iter_get_current(list_iterator_t *iter);

/**
 * @brief Insert an element between the last seen element and the current one. 
 * 
 * @param elem The new element to be added.
 * 
 * @note The current element afet the insertion will be the one just added.
 * 
 * @retval check `errno` for:
 * @retval - EINVAL
 * @retval - ENOMEM 
 */
void list_iter_insert(list_iterator_t *iter, void *elem);

/**
 * @brief Remove the current element at the iteration.
 * 
 * @note The current element after the deletion will be the one that was
 * coming after this one.
 * 
 * @return `void *` The element removed.
 * 
 * @retval `NULL` on error; check `errno` for:
 * @retval - EINVAL
 * @retval - ENODATA 
 */
void *list_iter_remove(list_iterator_t *iter);

#endif // _LIST_H