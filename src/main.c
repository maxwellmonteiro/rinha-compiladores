#include "transpiler.h"
#include "template.h"
#include "util/fn_list.h"
#include "util/callee_list.h"
#include "util/context.h"
#include <stdio.h>
#include <jansson.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FUNCTIONS 32
#define MAX_CALLEE 128
#define MAX_VARIABLES 128

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("uso: rinha-compiladores <arquivo da AST em formato json>\n");
        exit(EXIT_FAILURE);
    }

    json_t *json = json_load_file(argv[1], 0, NULL);

    json_t *expression = json_object_get(json, "expression");
    Context *context = context_new(MAX_FUNCTIONS, MAX_CALLEE, MAX_VARIABLES);
    Scope *scope = scope_new(NULL);

    transpiler_parse(expression, context, scope);

    scope_free(scope);
    context_free(context);

    json_decref(expression);

    return EXIT_SUCCESS;
}
