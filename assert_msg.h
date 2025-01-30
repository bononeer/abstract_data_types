#include <stdio.h>
#include <stdlib.h>

#define assert_msg(cond, msg) \
    if (!(cond)) { \
        fprintf(stderr, "%s", msg); \
        abort(); \
    }
