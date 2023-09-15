#include "transpiler.h"
#include "template.h"
#include <stdio.h>
#include <jansson.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SRC_BUFFER 16384
#define SIZE_OF_RETURN 8
#define PADDING_SIZE 20
#define MAX_FUNCTIONS 32
#define MAX_ARGS_LEN 256
#define MAX_PARAMS_LEN 256
#define MAX_CMD_BUFFER 256

void process_callees(FnList *fn_list, CalleeList *callee_list);
char *build_functions_template(FnList *fn_list);

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("uso: rinha-compiladores <arquivo da AST em formato json>\n");
        exit(EXIT_FAILURE);
    }

    json_t *json = json_load_file(argv[1], 0, NULL);

    json_t *expression = json_object_get(json, "expression");
    Context *context = context_new(MAX_FUNCTIONS);
    IR *result = transpiler_parse(expression, context);

    char main_result[MAX_SRC_BUFFER];

    process_callees(context->fn_list, context->callee_list);
    sprintf(main_result, main_template, build_functions_template(context->fn_list), result->exp);

    context_free(context);

    char command[MAX_CMD_BUFFER];
    sprintf(command, "gcc -pipe -O3 a.c -o a.o");

    FILE *file = fopen("a.c", "w");
    fwrite(main_result, sizeof(char), strlen(main_result), file);
    fclose(file);
    int ret = system(command);
    ret = system("./a.o");

    transpiler_free_ir(result);

    return ret;
}


// If there was a called to a not declared function, declare it here
void process_callees(FnList *fn_list, CalleeList *callee_list) {
    Fn *fn;
    for (int i = 0; i < callee_list->size; i++) {
        if (fn_list->find(fn_list, callee_list->values[i]->name) == NULL) {
            fn = malloc(sizeof(Fn));
            strncpy(fn->name, callee_list->values[i]->name, FUNCTION_MAX_NAME);
            sprintf(fn->src, "void %.128s(%.512s) {}", fn->name, callee_list->values[i]->args);
            fn_list->push(fn_list, fn);
        }
    }
}

char *build_functions_template(FnList *fn_list) {
    char *buffer = malloc(sizeof(char) * MAX_SRC_BUFFER);
    buffer[0] = 0;
    for (int i = 0; i < fn_list->size; i++) {
        strcat(buffer, fn_list->values[i]->src);
    }
    return buffer;
}