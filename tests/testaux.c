#include <string.h>
#include "testaux.h"

#define MAX_BUFF 1024
#define MAX_LOGS 64

/* ############################### Structure ################################ */

struct test {
    bool key;
    char name[MAX_BUFF];
    char *err_logs[MAX_LOGS];
    unsigned char indx;
};

static struct test curr_test_inst;
static struct test *curr_test = &curr_test_inst;

/* ############################# Aux Operations ############################# */

void test_check(bool condition, char *test_log) {
    if (condition) return;
    
    curr_test->key = false;
    if (curr_test->indx == MAX_LOGS) {
        errno = ENOSPC;
        return;
    }
    curr_test->err_logs[curr_test->indx++] = test_log ? test_log : "No log provided";
}

/* ############################ Test Operations ############################# */

void test_start(char *test_log) {
    errno = 0;
    if (strlen(test_log) >= MAX_BUFF) {
        errno = E2BIG;
        printf("\033[1;31mFAIL:\033[0m\n Test log too long.");
        return;
    }

    memset(curr_test, 0, sizeof(struct test)); 

    curr_test->key = true;
    strncpy(curr_test->name, test_log, (strlen(test_log) + 1) * sizeof(char));
}

void test_true(bool truey_cond, char *test_log) {
    test_check(truey_cond, test_log);
}

void test_false(bool falsey_cond, char *test_log) {
    test_check(!falsey_cond, test_log);
}

void test_errno_with(int err, char *test_log) {
    test_check(errno == err, test_log);
    errno = 0;
}

void test_equals(void *e1, void *e2, cmp_func_t cmp, char *test_log) {
    test_check(cmp(e1, e2) == 0, test_log);
}

void test_not_equals(void *e1, void *e2, cmp_func_t cmp, char *test_log) {
    test_check(cmp(e1, e2) != 0, test_log);
}

void test_lesser_than(void *e1, void *e2, cmp_func_t cmp, char *test_log) {
    test_check(cmp(e1, e2) < 0, test_log);
}

void test_greater_than(void *e1, void *e2, cmp_func_t cmp, char *test_log) {
    test_check(cmp(e1, e2) > 0, test_log);
}

void test_not_null(void *ptr, char *test_log) {
    test_check(ptr != NULL, test_log);
}

void test_end(void) {
    char *result = curr_test->key ? "\033[1;32mPASS:\033[0m" : "\033[1;31mFAIL:\033[0m";
    printf("%s %s\n", result, curr_test->name);

    if (curr_test->key) return;

    for (unsigned char i = 0 ; i < curr_test->indx ; i++) {
        printf("\t%s\n", curr_test->err_logs[i]);
    }
}

/* ########################### Compare Functions ############################ */

int intcmp(const void *e1, const void *e2) {
    return *(int *) e1 - *(int *) e2;
}

int floatcmp(const void *e1, const void *e2) {
    float res = *(float *) e1 - *(float *) e2;

    return res == 0 ? 0 : res > 0 ? 1 : -1;
}

int vstrcmp(const void *e1, const void *e2) {
    return strcmp((const char *) e1, (const char *) e2);
}

int sizecmp(const void *e1, const void *e2) {
    return (int) (*(size_t *) e1 - *(size_t *) e2);
}