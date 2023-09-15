#include "context.h"
#include "fn_list.h"
#include <stdlib.h>
#include <string.h>


Context *context_new(size_t size) {
    Context *context = malloc(sizeof(Context));

    context->fn_list = fn_list_new(size);
    context->callee_list = callee_list_new(size);
    context->place_return = false;

    return context;
}

void context_free(Context *context) {
    context->fn_list->destroy(context->fn_list);
    free(context);
}
