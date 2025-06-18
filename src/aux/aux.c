#include "aux.h"

/* ############################# Aux Operations ############################# */

bool null_set_err(void *ptr, int err) {
    if (!ptr) {
        errno = err;
        return true;
    }
    
    return false;
}

/* ############################### Operations ############################### */

bool err_null_ptr(void *ptr) {
    return null_set_err(ptr, EINVAL);
}

bool err_no_mem(void *ptr) {
    return null_set_err(ptr, ENOMEM);
}
