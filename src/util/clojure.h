#include "binding_list.h"
#include "fn_list.h"

#ifndef CLOJURE_H
#define CLOJURE_H

#define MAX_BINDINGS 64

typedef struct Clojure {
    Fn *fn;
    BindingList *binding_list;
} Clojure;

extern Clojure *clojure_new(Fn *fn, size_t size);
extern void clojure_free(Clojure *clojure);
extern void clojure_append(Clojure *self, Clojure *other);

#endif