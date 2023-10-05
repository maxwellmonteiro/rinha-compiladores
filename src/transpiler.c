#include "transpiler.h"
#include "term/term.h"
#include "term/binary_term.h"
#include "util/context.h"
#include "util/scope.h"
#include <stdio.h>
#include <jansson.h>
#include <string.h>
#include <stdbool.h>

Fn *transpiler_create_fn_if_absent(Context *context, const char *name);
size_t transpiler_get_init_pos_params(Scope *scope);

_DynamicType transpiler_parse(json_t *expression, Context *context, Scope *scope) {
    json_t *kind_obj = json_object_get(expression, "kind");
    const char *kind_str = json_string_value(kind_obj);

    _DynamicType result;

    if (kind_str != NULL) {
        uint16_t kind = term_get_kind(kind_str);

        switch (kind) {
            case Binary: {
                json_t *lhs = json_object_get(expression, "lhs");
                _DynamicType result_lhs = transpiler_parse(lhs, context, scope);

                json_t *op = json_object_get(expression, "op");
                
                json_t *rhs = json_object_get(expression, "rhs");
                _DynamicType result_rhs = transpiler_parse(rhs, context, scope);

                binary_func_t op_func = binary_term_get_op_func(binary_term_get_op(json_string_value(op)));

                result = op_func(result_lhs, result_rhs);
                break;
            }
            case If: {
                json_t *condition = json_object_get(expression, "condition");

                _DynamicType result_condition = transpiler_parse(condition, context, scope);
                
                _DynamicType result_if;
                if ((bool) result_condition.value) {
                    json_t *then = json_object_get(expression, "then");
                    result_if = transpiler_parse(then, context, scope);
                } else {
                    json_t *otherwise = json_object_get(expression, "otherwise");
                    result_if = transpiler_parse(otherwise, context, scope);
                }
                result = result_if;
                break;
            }
            case Var: {
                json_t *value = json_object_get(expression, "text");
                const char *value_str = json_string_value(value);
                Binding *bindging = scope->clojure->binding_list->find(scope->clojure->binding_list, value_str);
                result = bindging->value;
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
                    result_value = transpiler_parse(value, context, scope);
                    Binding *binding = scope->clojure->binding_list->find(scope->clojure->binding_list, result_name);
                    if (binding != NULL) {
                        binding->value = result_value;
                    } else {
                        binding = binding_new(result_name, result_value);
                        scope->clojure->binding_list->push(scope->clojure->binding_list, binding);
                    }
                } else {
                    Fn *fn = fn_new(result_name, value);
                    context->fn_list->push(context->fn_list, fn);
                    result_value.type = _DT_FUNC;
                }
                result = result_value;
                break;
            }
            case Function: {
                json_t *parameters = json_object_get(expression, "parameters");
                json_t *param;
                int i;
                size_t init_pos_params = transpiler_get_init_pos_params(scope);
                json_array_foreach(parameters, i, param) {
                    json_t *text = json_object_get(param, "text");
                    const char *result_param = json_string_value(text);

                    if ((init_pos_params + i) < scope->clojure->binding_list->size) {
                        binding_set_name(scope->clojure->binding_list->values[init_pos_params + i], result_param);
                    }
                }
                json_t *value = json_object_get(expression, "value");
                result = transpiler_parse(value, context, scope);
                break;
            }
            case Call: {
                json_t *callee = json_object_get(expression, "callee");
                json_t *callee_text = json_object_get(callee, "text");

                Fn *fn = transpiler_create_fn_if_absent(context, json_string_value(callee_text));

                json_t *arguments = json_object_get(expression, "arguments");

                json_t *arg;
                int i, j = 0;
                _DynamicType result_arg;

                Scope *new_scope = scope_new(fn);
                clojure_append(new_scope->clojure, scope->clojure);
                json_array_foreach(arguments, i, arg) {
                    result_arg = transpiler_parse(arg, context, scope);
                    Binding *binding = binding_new(NULL, result_arg);
                    new_scope->clojure->binding_list->push(new_scope->clojure->binding_list, binding);
                }
                result = transpiler_parse(fn->src, context, new_scope);
                scope_free(new_scope);
                break;
            } case Print: {
                json_t *value = json_object_get(expression, "value");

                _DynamicType result_value = transpiler_parse(value, context, scope);

                result = _dt_do_print(result_value);
                break;
            } case Tuple: {
                json_t *first = json_object_get(expression, "first");
                _DynamicType result_first = transpiler_parse(first, context, scope);

                json_t *second = json_object_get(expression, "second");                
                _DynamicType result_second = transpiler_parse(second, context, scope);

                result = _dt_new_tuple(result_first, result_second);
                break;
            } case First: {
                json_t *value = json_object_get(expression, "value");
                _DynamicType result_value = transpiler_parse(value, context, scope);

                result = _dt_get_tuple_first(result_value);
                break;
            }
            case Second: {
                json_t *value = json_object_get(expression, "value");
                _DynamicType result_value = transpiler_parse(value, context, scope);

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
        result = transpiler_parse(next, context, scope);
    }
    return result;
}

Fn *transpiler_create_fn_if_absent(Context *context, const char *name) {
    Fn *fn = context->fn_list->find(context->fn_list, name);

    if (fn == NULL) {
        fn = fn_new(name, NULL);
        context->fn_list->push(context->fn_list, fn);
    }
    return fn;
}

size_t transpiler_get_init_pos_params(Scope *scope) {
    int i;
    for (i = 0; i < scope->clojure->binding_list->size && scope->clojure->binding_list->values[i]->name != NULL; i++);
    return i;
}