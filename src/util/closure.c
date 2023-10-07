#include "closure.h"

Closure *closure_new(json_t *json) {
    Closure *closure = malloc(sizeof(Closure));
    closure->json = json;
    closure->binding_list = binding_list_new(BINDING_LIST_INITIAL_SIZE);
    return closure;
}

void closure_free(Closure *closure) {
    closure->binding_list->destroy(closure->binding_list);
    closure->binding_list = NULL;
    free(closure);
}
