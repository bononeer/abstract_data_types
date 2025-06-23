#ifndef _ADTCOMM_H
#define _ADTCOMM_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

/**
 * @brief Function that describes how to destroy dynamically allocated memory.
 * 
 * @param elem The generic pointer to the memory.
 * @note Destroy functions for any ADT must be able to be treated as `destroy_func_t`.
 */
typedef void (*destroy_func_t)(void *elem);

/**
 * @brief Function that describes how to compare two generic elements.
 * 
 * @param e1, e2 Generic pointer to a value to be compared.
 * @return `int` A number which indicates the result
 * 
 * @retval `< 0` If e1 < e2.
 * @retval `0`   If e1 == e2.
 * @retval `> 0` If e1 > e2.
 */
typedef int (*cmp_func_t)(const void *e1, const void *e2);

/**
 * @brief function that evaluates either true or false for a given generic element.
 * 
 * @param elem Generic pointer to a value to be evaluated.
 * @param extra Generic pointer to an additional value, to help the logic of the function.
 * @return `bool`
 */
typedef bool (*smpl_visit_func_t)(const void *elem, const void *extra);

/**
 * @brief The type for a hasheable value.
 */
typedef const unsigned char * hkey_t;

/**
 * @brief function that evaluates either true or false for a given key and its value.
 * 
 * @param key A `hkey_t` to be evaluated.
 * @param value Generic pointer to a value to be evaluated.
 * @param extra Generic pointer to an additional value, to help the logic of the function.
 * @return `bool`
 */
typedef bool (*cmpl_visit_func_t)(hkey_t key, const void *value, const void *extra);

#endif // _ADTCOMM_H