#include "interpreter.h"
#include "term/term.h"
#include "term/binary_term.h"
#include "util/closure.h"
#include <stdio.h>
#include <jansson.h>
#include <string.h>
#include <stdbool.h>

static size_t interpreter_get_init_pos_params(Closure *closure);
static bool interpreter_is_function(json_t *json);
static _DynamicType interpreter_parse_function(json_t *expression, Closure *closure);

_DynamicType interpreter_parse(json_t *expression, Closure *closure) {
    json_t *kind_obj = json_object_get(expression, "kind");
    const char *kind_str = json_string_value(kind_obj);

    _DynamicType result;

    if (kind_str != NULL) {
        uint16_t kind = term_get_kind(kind_str);

        switch (kind) {
            case Binary: {
                json_t *lhs = json_object_get(expression, "lhs");
                _DynamicType result_lhs = interpreter_parse(lhs, closure);

                json_t *op = json_object_get(expression, "op");
                
                json_t *rhs = json_object_get(expression, "rhs");
                _DynamicType result_rhs = interpreter_parse(rhs, closure);

                binary_func_t op_func = binary_term_get_op_func(binary_term_get_op(json_string_value(op)));

                result = op_func(result_lhs, result_rhs);
                break;
            }
            case If: {
                json_t *condition = json_object_get(expression, "condition");

                _DynamicType result_condition = interpreter_parse(condition, closure);
                
                _DynamicType result_if;
                if ((bool) result_condition.value) {
                    json_t *then = json_object_get(expression, "then");
                    result_if = interpreter_parse(then, closure);
                } else {
                    json_t *otherwise = json_object_get(expression, "otherwise");
                    result_if = interpreter_parse(otherwise, closure);
                }
                result = result_if;
                break;
            }
            case Var: {
                json_t *value = json_object_get(expression, "text");
                const char *value_str = json_string_value(value);
                Binding *binding = closure->binding_list->find(closure->binding_list, value_str);
                result = binding->value;
                break;
            }
            case Int: {
                json_t *value = json_object_get(expression, "value");
                result = _dt_new_int(json_integer_value(value));
                break;
            }
            case Str: {
                json_t *value = json_object_get(expression, "value");
                result = _dt_new_str(json_string_value(value));
                break;
            }
            case Bool: {
                json_t *value = json_object_get(expression, "value");
                result = _dt_new_bool(json_boolean_value(value));
                break;
            }
            case Let: {
                json_t *name = json_object_get(expression, "name");
                json_t *text = json_object_get(name, "text");
                const char *result_name = json_string_value(text);

                json_t *value = json_object_get(expression, "value");

                uint16_t inner_kind = term_get_kind(json_string_value(json_object_get(value, "kind")));

                _DynamicType result_value;
                if (inner_kind != Function) {
                    result_value = interpreter_parse(value, closure);
                } else {
                    result_value.type = _DT_FUNC;
                    result_value.value = (size_t) value;
                }
                Binding *binding = closure->binding_list->find(closure->binding_list, result_name);
                if (binding != NULL) {
                    binding->value = result_value;
                } else {
                    binding = binding_new(result_name, result_value);
                    closure->binding_list->push(closure->binding_list, binding);
                }
                result = result_value;
                break;
            }
            case Function: {
                result = interpreter_parse_function(expression, closure);
                break;
            }
            case Call: {
                json_t *callee = json_object_get(expression, "callee");
                json_t *callee_text = json_object_get(callee, "text");

                json_t *arguments = json_object_get(expression, "arguments");

                json_t *arg;
                int i;
                _DynamicType result_arg;

                Closure *new_closure = closure_new(interpreter_parse_function);
                closure_append(new_closure, closure);
                json_array_foreach(arguments, i, arg) {
                    if (interpreter_is_function(arg)) { // anonymous function passed as argument
                        result_arg.type = _DT_FUNC;
                        result_arg.value = (size_t) arg;
                    } else {
                        result_arg = interpreter_parse(arg, closure);
                    }
                    Binding *binding = binding_new(NULL, result_arg);
                    new_closure->binding_list->push(new_closure->binding_list, binding);
                }
                Binding *binding_fn = closure->binding_list->find(closure->binding_list, json_string_value(callee_text));
                if (binding_fn != NULL) {
                    result = new_closure->call((json_t *) binding_fn->value.value, new_closure);
                } else { // inline function. A linguagem proposta permite chamar uma funcao sem declará-la antes
                    result = interpreter_parse(json_object_get(expression, "next"), new_closure);
                }
                closure_free(new_closure);
                break;
            } case Print: {
                json_t *value = json_object_get(expression, "value");

                _DynamicType result_value = interpreter_parse(value, closure);

                result = _dt_do_print(result_value);
                break;
            } case Tuple: {
                json_t *first = json_object_get(expression, "first");
                _DynamicType result_first = interpreter_parse(first, closure);

                json_t *second = json_object_get(expression, "second");                
                _DynamicType result_second = interpreter_parse(second, closure);

                result = _dt_new_tuple(result_first, result_second);
                break;
            } case First: {
                json_t *value = json_object_get(expression, "value");
                _DynamicType result_value = interpreter_parse(value, closure);

                result = _dt_get_tuple_first(result_value);
                break;
            }
            case Second: {
                json_t *value = json_object_get(expression, "value");
                _DynamicType result_value = interpreter_parse(value, closure);

                result = _dt_get_tuple_second(result_value);
                break;
            }
            default:
                printf("Falha durante o parse da AST\n");
                exit(EXIT_FAILURE);
        }
    }

    json_t *next = json_object_get(expression, "next");
    if (next != NULL) {
        result = interpreter_parse(next, closure);
    }
    return result;
}

static size_t interpreter_get_init_pos_params(Closure *closure) {
    int i;
    for (i = 0; i < closure->binding_list->size && closure->binding_list->values[i]->name != NULL; i++);
    return i;
}

static bool interpreter_is_function(json_t *json) {
    json_t *kind_obj = json_object_get(json, "kind");
    const char *kind_str = json_string_value(kind_obj);

    return kind_str != NULL && term_get_kind(kind_str) == Function;
}

static _DynamicType interpreter_parse_function(json_t *expression, Closure *closure) {
    json_t *parameters = json_object_get(expression, "parameters");
    json_t *param;
    int i;
    size_t init_pos_params = interpreter_get_init_pos_params(closure);
    json_array_foreach(parameters, i, param) {
        json_t *text = json_object_get(param, "text");
        const char *result_param = json_string_value(text);

        if ((init_pos_params + i) < closure->binding_list->size) {
            binding_set_name(closure->binding_list->values[init_pos_params + i], result_param);
        }
    }
    json_t *value = json_object_get(expression, "value");
    return interpreter_parse(value, closure);
}