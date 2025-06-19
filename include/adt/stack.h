#ifndef _STACK_H
#define _STACK_H

#include "adt/adtcomm.h"

/* ############################### Structure ################################ */

/**
 * @brief Data structure that follows the LIFO principle.
 */
typedef struct dynamic_stack stack_t;

/* ############################### Operations ############################### */

/**
 * @brief Initialize a new empty Stack.
 * 
 * @param elem_destroy Describes the way to destroy the elements stored when 
 * `stack_destroy` is used with this Stack.
 * @return Pointer to the new Stack.
 * 
 * @retval `NULL` on error; check `errno` for:
 * @retval - ENOMEM
 */
stack_t *stack_create(destroy_func_t elem_destroy);

/**
 * @brief Free the Stack's memory and all the elements stored inside.
 * 
 * @retval check `errno` for:
 * @retval - EINVAL
 */
void stack_destroy(void *stack);

/**
 * @brief Check if the Stack is empty.
 * 
 * @retval `true` if the Stack is empty.
 * @retval `false` otherwise; check `errno` for:
 * @retval - EINVAL
 */
bool stack_is_empty(stack_t *stack);

/**
 * @brief Add a new element to the top of the Stack.
 * 
 * @param elem The new element to be stored.
 * 
 * @retval check `errno` for:
 * @retval - EINVAL
 * @retval - ENOMEM
 */
void stack_push(stack_t *stack, void *elem);

/**
 * @brief Remove the top element of the Stack.
 * 
 * @return `void *` The removed element.
 * 
 * @retval `NULL` on error; check `errno` for:
 * @retval - EINVAL
 * @retval - ENOMEM
 * @retval - ENODATA
 */
void *stack_pop(stack_t *stack);

/**
 * @brief Get the top element of the Stack.
 * 
 * @return `void *` The top element.
 * 
 * @retval `NULL` on error; check `errno` for:
 * @retval - EINVAL
 * @retval - ENODATA
 */
void *stack_top(stack_t *stack);

#endif // _STACK_H
