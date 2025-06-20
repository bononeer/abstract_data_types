#ifndef _LIST_AUX_H
#define _LIST_AUX_H

#include "adt/list.h"
#include "aux/aux.h"
#include "aux/node.h"

/* ############################### Operations ############################### */

bool err_empty_list(list_t *list);

bool err_finsihed_list_iter(list_iterator_t *iter);

#endif // _LIST_AUX_H