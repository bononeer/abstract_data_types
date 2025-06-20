/**
 * @file testaux.h
 * @author bononeer (bonoe.neer@gmail.com)
 * @brief Test your code.
 * 
 * Provides functions to test your implementations.
 * Also brings some `cmp_func_t` so you don't have to define them.
 */

#ifndef _TESTAUX_H
#define _TESTAUX_H

#include <stdio.h>
#include "adt/adtcomm.h"

#define MID_AMOUNT 100
#define BULK_AMOUNT 100000

/* ############################ Test Operations ############################# */

/**
 * @brief Start a new test.
 * 
 * @param test_log The test case name.
 */
void test_start(char *test_log);

/**
 * @brief Test that the condition is true.
 * 
 * @param truey_cond Condition to check.
 * @param test_log Describes what is being tested.
 */
void test_true(bool truey_cond, char *test_log);

/**
 * @brief Test that the condition is false.
 * 
 * @param truey_cond Condition to check.
 * @param test_log Describes what is being tested.
 */
void test_false(bool falsey_cond, char *test_log);

/**
 * @brief Test that errno was set to a certain value.
 * 
 * @param truey_cond The error value to check.
 * @param test_log Describes what is being tested.
 * 
 * @note Sets errno back to 0.
 */
void test_errno_with(int err, char *test_log);

/**
 * @brief Test that two elements have the same value.
 * 
 * @param e1, e2 Element to be checked.
 * @param cmp Function used to compare the elements.
 * @param test_log Describes what is being tested.
 */
void test_equals(void *e1, void *e2, cmp_func_t cmp, char *test_log);

/**
 * @brief Test that two elements don't have the same value.
 * 
 * @param e1, e2 Element to be checked.
 * @param cmp Function used to compare the elements.
 * @param test_log Describes what is being tested.
 */
void test_not_equals(void *e1, void *e2, cmp_func_t cmp, char *test_log);

/**
 * @brief Test that the first element is lesser than the second one.
 * 
 * @param e1, e2 Element to be checked.
 * @param cmp Function used to compare the elements.
 * @param test_log Describes what is being tested.
 */
void test_lesser_than(void *e1, void *e2, cmp_func_t cmp, char *test_log);

/**
 * @brief Test that the first element is greater than the second one.
 * 
 * @param e1, e2 Element to be checked.
 * @param cmp Function used to compare the elements.
 * @param test_log Describes what is being tested.
 */
void test_greater_than(void *e1, void *e2, cmp_func_t cmp, char *test_log);

/**
 * @brief Test that the pointer is not null.
 * 
 * @param ptr Pointer to be checked.
 * @param test_log Describes what is being tested.
 */
void test_not_null(void *ptr, char *test_log);

/**
 * @brief Stops the test case, check that evverything went as expected.
 * 
 * Will print all failed test_logs.
 */
void test_end(void);

/* ########################### Compare Functions ############################ */

/**
 * @brief Compare two integer numbers.
 * 
 * @param e1, e2 Pointer to an integer.
 * @return `int` A number which indicates the result
 * 
 * @retval `< 0` If e1 < e2.
 * @retval `0`   If e1 == e2.
 * @retval `> 0` If e1 > e2.
 */
int intcmp(const void *e1, const void *e2);

/**
 * @brief Compare two floating point numbers.
 * 
 * @param e1, e2 Pointer to a float.
 * @return `int` A number which indicates the result
 * 
 * @retval `< 0` If e1 < e2.
 * @retval `0`   If e1 == e2.
 * @retval `> 0` If e1 > e2.
 */
int floatcmp(const void *e1, const void *e2);

/**
 * @brief Compare two strings.
 * 
 * @param e1, e2 Pointer to a char.
 * @return `int` A number which indicates the result
 * 
 * @retval `< 0` If e1 < e2.
 * @retval `0`   If e1 == e2.
 * @retval `> 0` If e1 > e2.
 */
int vstrcmp(const void *e1, const void *e2);

/**
 * @brief Compare two sizes.
 * 
 * @param e1, e2 Pointer to a size_t.
 * @return `int` A number which indicates the result
 * 
 * @retval `< 0` If e1 < e2.
 * @retval `0`   If e1 == e2.
 * @retval `> 0` If e1 > e2.
 */
int sizecmp(const void *e1, const void *e2);

#endif // _TESTAUX_H