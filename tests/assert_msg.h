#ifndef _ASSERT_MSG_H
#define _ASSERT_MSG_H

#include <stdio.h>
#include <stdlib.h>

#define AMOUNT 50
#define BULK_AMOUNT 10000

#define assert_msg(cond, msg) \
    if (!(cond)) { \
        fprintf(stderr, "%s\n", msg); \
        abort(); \
    }

#endif // _ASSERT_MSG_H