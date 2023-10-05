#include "fn_list.h"
#include "callee_list.h"
#include "dynamic_type.h"
#include <stdbool.h>
#include <jansson.h>
#include <inttypes.h>

#ifndef CONTEXT_H
#define CONTEXT_H

#define MAX_ARGS 64

typedef struct Context {
    FnList *fn_list;
    CalleeList *callee_list;
} Context;


extern Context *context_new(size_t max_func, size_t max_callee, size_t max_var);
extern void context_free(Context *context);

#endif