#include "clojure.h"
#include "fn_list.h"

#ifndef SCOPE_H
#define SCOPE_H

typedef struct Scope {
    Clojure *clojure;
} Scope;

extern Scope *scope_new(Fn *fn);
extern void scope_free(Scope *scope);

#endif