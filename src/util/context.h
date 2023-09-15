#include "fn_list.h"
#include "callee_list.h"
#include <stdbool.h>
#include <jansson.h>
#include <inttypes.h>

#ifndef CONTEXT_H
#define CONTEXT_H

typedef struct Context {
    FnList *fn_list;
    CalleeList *callee_list;
    bool place_return;
} Context;


extern Context *context_new(size_t size);
extern void context_free(Context *context);

#endif