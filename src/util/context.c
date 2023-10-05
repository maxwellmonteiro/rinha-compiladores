#include "context.h"
#include "fn_list.h"
#include "var_list.h"
#include <stdlib.h>
#include <string.h>


Context *context_new(size_t max_func, size_t max_callee, size_t max_var) {
    Context *context = malloc(sizeof(Context));

    context->fn_list = fn_list_new(max_func);
    context->callee_list = callee_list_new(max_callee);

    return context;
}

void context_free(Context *context) {
    context->fn_list->destroy(context->fn_list);
    free(context);
}
