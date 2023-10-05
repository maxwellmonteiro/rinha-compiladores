#include <inttypes.h>
#include <stdlib.h>

#ifndef  DYNAMIC_TYPE_H
#define DYNAMIC_TYPE_H

#define _DT_INT 0
#define _DT_STR 1
#define _DT_BOOL 2
#define _DT_TUPLE 3
#define _DT_FUNC 4

typedef struct _DynamicType {
    uint8_t type;
    size_t value;
} _DynamicType;

typedef struct _DynamicTuple {
    _DynamicType first;
    _DynamicType second;
} _DynamicTuple;

#endif