#ifndef _ASSERT_MSG_H
#define _ASSERT_MSG_H

#include <stdio.h>
#include <stdlib.h>

#define assert_msg(cond, msg) \
    if (!(cond)) { \
        fprintf(stderr, "%s\n", msg); \
        abort(); \
    }

#endif // _ASSERT_MSG_H