#include "binding_list.h"
#include <jansson.h>

#ifndef CLOSURE_H
#define CLOSURE_H

#define MAX_BINDINGS 64

typedef struct Closure {
    // _DynamicType (*call)(json_t *, struct Closure *);
    json_t *json;
    BindingList *binding_list;
} Closure;

typedef _DynamicType (*closure_call_t)(json_t *, Closure *);

extern Closure *closure_new(json_t *json);
extern void closure_free(Closure *closure);

#endif