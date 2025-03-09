#ifndef _ADDITIONAL_TYPES_H
#define _ADDITIONAL_TYPES_H

/* A compare function that returns the following in different cases:
    - `< 0` if the first element is lesser than the second one. 
    - `== 0` if both elements are equal.
    - `> 0` if the first element is greater than the second one. */
typedef int (*cmp_func_t)(const char*, const char*);
/* A function for the internal iterators which returns a boolean value to decide if the 
iteration continues or not. It receives an `extra` parameter that can be NULL. */
typedef bool (*visit_func_t)(const char*, void*, void *extra);
// A function that defines how to destroy a given generic element.
typedef void (*destroy_func_t)(void*);

#endif // _ADDITIONAL_TYPES_H