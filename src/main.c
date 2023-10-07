#include "interpreter.h"
#include <stdio.h>
#include <jansson.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("uso: rinha-compiladores <arquivo da AST em formato json>\n");
        exit(EXIT_FAILURE);
    }

    json_t *json = json_load_file(argv[1], 0, NULL);

    json_t *expression = json_object_get(json, "expression");
    Closure *closure = closure_new(expression);

    interpreter_parse(expression, closure);

    closure_free(closure);

    json_decref(expression);

    return EXIT_SUCCESS;
}
