#include "transpiler.h"
#include "term/term.h"
#include "term/binary_term.h"
#include "util/context.h"
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

static char *transpiler_get_empty_str() {
    char *empty_str = malloc(sizeof(char));
    empty_str[0] = 0;
    return empty_str;
}

IR *transpiler_alloc_ir(size_t len) {
    IR *ir = malloc(sizeof(IR));
    ir->exp = (char *) malloc(sizeof(char) * len);
    ir->exp[0] = 0;
    ir->type = IR_UNDEFINED;
    return ir;
}

void transpiler_free_ir(IR *ir) {
    if (ir != NULL) {
        free(ir->exp);
        ir->exp = NULL;
        free(ir);
    }
}

void transpiler_place_return(IR *ir) {
    char *new_exp = malloc(strlen(ir->exp) + SIZE_OF_RETURN  + PADDING_SIZE);
    sprintf(new_exp, "return %s;", ir->exp);
    free(ir->exp);
    ir->exp = new_exp;
}

bool transpiler_can_place_return(Context *context, json_t *json_node) {
    uint16_t _kind = term_get_kind(json_string_value(json_object_get(json_node, "kind")));
    return context->place_return && _kind != If && _kind != Let && (json_object_get(json_node, "next") == NULL);
}

uint8_t transpiler_eval_ir_type(IR *ir1, IR *ir2) {
    if (ir1->type == IR_STR || ir2->type == IR_STR) {
        return IR_STR;
    }
    if (ir1->type == IR_BOOL || ir2->type == IR_BOOL) {
        return IR_BOOL;
    }
    if (ir1->type == IR_INT && ir2->type == IR_INT) {
        return IR_INT;
    }
    return IR_UNDEFINED;
}

IR *transpiler_parse(json_t *expression, Context *context) {
    json_t *kind_obj = json_object_get(expression, "kind");
    const char *kind_str = json_string_value(kind_obj);

    IR *result = NULL;
    IR *result_final = NULL;

    if (kind_str != NULL) {
        uint16_t kind = term_get_kind(kind_str);

        switch (kind) {
            case Binary: {
                json_t *lhs = json_object_get(expression, "lhs");
                IR *result_lhs = transpiler_parse(lhs, context);

                json_t *op = json_object_get(expression, "op");            
                const char *op_str = json_string_value(op);
                const char *result_op = binary_term_get_op_target(binary_term_get_op(op_str));
                
                json_t *rhs = json_object_get(expression, "rhs");
                IR *result_rhs = transpiler_parse(rhs, context);

                result = transpiler_alloc_ir(strlen(result_lhs->exp) + strlen(result_op) + strlen(result_rhs->exp) + PADDING_SIZE);
                sprintf(result->exp, "%s(%s, %s)", result_op, result_lhs->exp, result_rhs->exp);

                result->type = transpiler_eval_ir_type(result_lhs, result_rhs);

                if (result->type == IR_UNDEFINED && binary_term_is_logic_op(op_str)) {
                    result->type = IR_BOOL;
                }

                transpiler_free_ir(result_lhs);
                transpiler_free_ir(result_rhs);
                break;
            }
            case If: {
                json_t *condition = json_object_get(expression, "condition");

                // do not place return in condition expr
                bool state_place_return = context->place_return;
                context->place_return = false;
                IR *result_condition = transpiler_parse(condition, context);
                
                context->place_return = state_place_return;

                json_t *then = json_object_get(expression, "then");
                IR *result_then = transpiler_parse(then, context);

                json_t *otherwise = json_object_get(expression, "otherwise");
                IR *result_otherwise = transpiler_parse(otherwise, context);

                if (context->place_return) {
                    if (json_object_get(then, "next") == NULL && term_get_kind(json_string_value(json_object_get(then, "kind"))) != If) {
                        transpiler_place_return(result_then);
                    }
                    if (json_object_get(otherwise, "next") == NULL && term_get_kind(json_string_value(json_object_get(otherwise, "kind"))) != If) {
                        transpiler_place_return(result_otherwise);
                    }
                }

                json_t *condition_kind = json_object_get(condition, "kind");
                if (condition_kind != NULL && term_get_kind(json_string_value(condition_kind)) == Var) {
                    result = transpiler_alloc_ir(strlen(result_condition->exp) + strlen(result_then->exp) + strlen(result_otherwise->exp) + PADDING_SIZE + 12);                
                    sprintf(result->exp, "if (_dt_get_bool(%s)) {%s} else {%s}", result_condition->exp, result_then->exp, result_otherwise->exp);
                } else {
                    result = transpiler_alloc_ir(strlen(result_condition->exp) + strlen(result_then->exp) + strlen(result_otherwise->exp) + PADDING_SIZE);                
                    sprintf(result->exp, "if (%s) {%s} else {%s}", result_condition->exp, result_then->exp, result_otherwise->exp);
                }

                result->type = transpiler_eval_ir_type(result_then, result_otherwise);

                transpiler_free_ir(result_condition);
                transpiler_free_ir(result_then);
                transpiler_free_ir(result_otherwise);
                break;
            }
            case Var: {
                json_t *value = json_object_get(expression, "text");
                const char *value_str = json_string_value(value);
                result = transpiler_alloc_ir(strlen(value_str));
                sprintf(result->exp, "%s", value_str);
                break;
            }
            case Int: {
                json_t *value = json_object_get(expression, "value");
                int32_t value_int = json_integer_value(value);
                result = transpiler_alloc_ir(12 + PADDING_SIZE);
                sprintf(result->exp, "_dt_new_int(%d)", value_int);
                result->type = IR_DT;
                break;
            }
            case Str: {
                json_t *value = json_object_get(expression, "value");
                const char *value_str = json_string_value(value);
                result = transpiler_alloc_ir(strlen(value_str) + PADDING_SIZE);
                sprintf(result->exp, "_dt_new_str(\"%s\")", value_str);
                result->type = IR_DT;
                break;
            }
            case Bool: {
                json_t *value = json_object_get(expression, "value");
                bool value_bool = json_boolean_value(value);
                result = transpiler_alloc_ir(5 + PADDING_SIZE);
                sprintf(result->exp, "_dt_new_bool(%s)", value_bool ? "true" : "false");
                result->type = IR_DT;
                break;
            }
            case Let: {
                json_t *name = json_object_get(expression, "name");
                json_t *text = json_object_get(name, "text");
                const char *result_name = json_string_value(text);
                

                json_t *value = json_object_get(expression, "value");
                IR *result_value = transpiler_parse(value, context);

                result = transpiler_alloc_ir(strlen(result_name) + strlen(result_value->exp) + PADDING_SIZE + PADDING_SIZE);

                if (result_value->type == IR_FUNCTION) {
                    Fn *fn = fn_new();
                    strncpy(fn->name, result_name, FUNCTION_MAX_NAME - 1);
                    sprintf(fn->src, "_DynamicType %s%s\n", result_name, result_value->exp);
                    context->fn_list->push(context->fn_list, fn);
                    result->exp = transpiler_get_empty_str();
                } else if (result_value->type == IR_BOOL) {
                    sprintf(result->exp, "_DynamicType %s = _dt_new_bool(%s);", result_name, result_value->exp);
                } else if (result_value->type == IR_STR) {
                    sprintf(result->exp, "_DynamicType %s = _dt_new_str(%s);", result_name, result_value->exp);
                } else if (result_value->type == IR_INT) {
                    sprintf(result->exp, "_DynamicType %s = _dt_new_int(%s);", result_name, result_value->exp);
                } else {
                    sprintf(result->exp, "_DynamicType %s = %s;", result_name, result_value->exp);
                }

                transpiler_free_ir(result_value);

                break;
            }
            case Function: {                        
                json_t *parameters = json_object_get(expression, "parameters");
                
                json_t *param;
                int i;
                IR *result_parameters = transpiler_alloc_ir(MAX_PARAMS_LEN);
                json_array_foreach(parameters, i, param) {
                    char temp_param[MAX_PARAMS_LEN];
                    json_t *text = json_object_get(param, "text");
                    const char *result_param = json_string_value(text);
                    if (i >= (json_array_size(parameters) - 1)) {
                        sprintf(temp_param, "_DynamicType %s", result_param);
                        strcat(result_parameters->exp, temp_param);
                    } else {
                        sprintf(temp_param, "_DynamicType %s,", result_param);
                        strcat(result_parameters->exp, temp_param);
                    }
                }

                json_t *value = json_object_get(expression, "value");

                json_t *other = json_object_get(value, "other");
                if (other == NULL) { // is the last statement
                    context->place_return = true;
                }
                IR *result_value = transpiler_parse(value, context);
                if (transpiler_can_place_return(context, value)) {
                    transpiler_place_return(result_value);
                }
                context->place_return = false;

                result = transpiler_alloc_ir(strlen(result_parameters->exp) + strlen(result_value->exp) + PADDING_SIZE);
                sprintf(result->exp, "(%s) { %s }", result_parameters->exp, result_value->exp);
                result->type = IR_FUNCTION;

                transpiler_free_ir(result_parameters);
                transpiler_free_ir(result_value);

                break;
            }
            case Call: {
                json_t *callee = json_object_get(expression, "callee");
                IR *result_callee = transpiler_parse(callee, context);
                
                json_t *arguments = json_object_get(expression, "arguments");

                json_t *arg;
                int i;
                IR *result_arg;
                IR *result_arguments = transpiler_alloc_ir(MAX_ARGS_LEN);

                Callee *callee_fn;
                bool callee_found = context->callee_list->find(context->callee_list, result_callee->exp) != NULL;

                // Se não encontrou a declaração da função da lista de funções é uma chamada já declarando a função, exemplo f(a, b) {...}
                // Então vamos criar a funcão e inserir na lista de funções
                if (!callee_found) {
                    callee_fn = callee_new();
                    strcpy(callee_fn->name, result_callee->exp);
                }

                json_array_foreach(arguments, i, arg) {
                    char temp_args[CALLEE_MAX_ARGS];
                    result_arg = transpiler_parse(arg, context);
                    if (i >= (json_array_size(arguments) - 1)) {
                        strcat(result_arguments->exp, result_arg->exp);
                        if (!callee_found) {
                            sprintf(temp_args, "_DynamicType p%d", i);
                            strcat(callee_fn->args, temp_args);
                        }
                    } else {
                        strcat(result_arguments->exp, result_arg->exp);
                        strcat(result_arguments->exp, ",");
                        if (!callee_found) {
                            sprintf(temp_args, "_DynamicType p%d,", i);
                            strcat(callee_fn->args, temp_args);
                        }
                    }
                    transpiler_free_ir(result_arg);
                }

                if (!callee_found) {
                    context->callee_list->push(context->callee_list, callee_fn);
                }

                result = transpiler_alloc_ir(strlen(result_arguments->exp) + strlen(result_callee->exp) + PADDING_SIZE);
                sprintf(result->exp, "%s(%s)", result_callee->exp, result_arguments->exp);

                transpiler_free_ir(result_arguments);
                break;
            } case Print: {
                json_t *value = json_object_get(expression, "value");

                IR *result_value = transpiler_parse(value, context);
                result = transpiler_alloc_ir(strlen(result_value->exp) + PADDING_SIZE);

                sprintf(result->exp, "_dt_do_print(%s)", result_value->exp);

                transpiler_free_ir(result_value);
                break;
            } case Tuple: {
                json_t *first = json_object_get(expression, "first");
                IR *result_first = transpiler_parse(first, context);

                json_t *second = json_object_get(expression, "second");
                IR *result_second = transpiler_parse(second, context);

                result = transpiler_alloc_ir(strlen(result_first->exp) + strlen(result_second->exp) + PADDING_SIZE);

                sprintf(result->exp, "_dt_new_tuple(%s, %s)", result_first->exp, result_second->exp);
                result->type = IR_TUPLE;

                transpiler_free_ir(result_first);
                transpiler_free_ir(result_second);
                break;
            } case First: {
                json_t *value = json_object_get(expression, "value");
                IR *result_value = transpiler_parse(value, context);

                result = transpiler_alloc_ir(strlen(result_value->exp) + PADDING_SIZE);
                sprintf(result->exp, "_dt_get_tuple_first(%s)", result_value->exp);

                transpiler_free_ir(result_value);
                break;
            }
            case Second: {
                json_t *value = json_object_get(expression, "value");
                IR *result_value = transpiler_parse(value, context);

                result = transpiler_alloc_ir(strlen(result_value->exp) + PADDING_SIZE);
                sprintf(result->exp, "_dt_get_tuple_second(%s)", result_value->exp);

                transpiler_free_ir(result_value);
                break;
            }
            default:
                printf("Falha durante o parse da AST\n");
                exit(EXIT_FAILURE);
        }
        
        IR *result_next = NULL;
        size_t result_next_len = 0;
        json_t *next = json_object_get(expression, "next");
        if (next != NULL) {
            result_next = transpiler_parse(next, context);
            if (transpiler_can_place_return(context, next)) {
                transpiler_place_return(result_next);
            }
            result_next_len = strlen(result_next->exp);
        }
        result_final = transpiler_alloc_ir(strlen(result->exp) + result_next_len + PADDING_SIZE);
        sprintf(result_final->exp, "%s%s", result->exp, result_next != NULL ? result_next->exp : "");
        result_final->type = result->type;
        transpiler_free_ir(result);
        transpiler_free_ir(result_next);
    }

    return result_final;
}