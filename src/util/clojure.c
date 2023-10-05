#include "clojure.h"

Clojure *clojure_new(Fn *fn, size_t size) {
    Clojure *clojure = malloc(sizeof(Clojure));
    clojure->fn = fn;
    clojure->binding_list = binding_list_new(size);
    return clojure;
}

void clojure_free(Clojure *clojure) {
    clojure->binding_list->destroy(clojure->binding_list);
    clojure->binding_list = NULL;
    free(clojure);
}

void clojure_append(Clojure *self, Clojure *other) {
    for (int i = 0; i < other->binding_list->size; i++) {
        if (self->fn != other->fn) {
            Binding *binding = binding_new(other->binding_list->values[i]->name, other->binding_list->values[i]->value);
            self->binding_list->push(self->binding_list, binding);
        }
    }
}