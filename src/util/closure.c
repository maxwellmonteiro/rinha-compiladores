#include "closure.h"

Closure *closure_new(closure_call_t call) {
    Closure *closure = malloc(sizeof(Closure));
    closure->call = call;
    closure->binding_list = binding_list_new(BINDING_LIST_INITIAL_SIZE);
    return closure;
}

void closure_free(Closure *closure) {
    closure->binding_list->destroy(closure->binding_list);
    closure->binding_list = NULL;
    free(closure);
}

void closure_append(Closure *self, Closure *other) {
    for (int i = 0; i < other->binding_list->size; i++) {
        if (self->binding_list->find(self->binding_list, other->binding_list->values[i]->name) == NULL) {
            Binding *binding = binding_new(other->binding_list->values[i]->name, other->binding_list->values[i]->value);
            self->binding_list->push(self->binding_list, binding);
        }
    }
}