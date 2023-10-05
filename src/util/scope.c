#include "scope.h"

Scope *scope_new(Fn *fn) {
    Scope *scope = malloc(sizeof(Scope));
    scope->clojure = clojure_new(fn, MAX_BINDINGS); 
    return scope;
}

void scope_free(Scope *scope) {
    clojure_free(scope->clojure);
    scope->clojure = NULL;
    free(scope);
}