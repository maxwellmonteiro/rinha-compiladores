char *main_template = 
    "#include <stdio.h>\n"
    "#include <stdbool.h>\n"
    "#include <inttypes.h>\n"
    "#include <string.h>\n"
    "#include <stdlib.h>\n"
    "#define _DT_INT 0\n"
    "#define _DT_STR 1\n"
    "#define _DT_BOOL 2\n"
    "#define _DT_TUPLE 3\n"   
    "typedef struct _DynamicType {\n" 
      "uint8_t type;\n"
      "size_t value;\n"
    "} _DynamicType;\n"
    "typedef struct _DynamicTuple {\n" 
      "_DynamicType first;\n"
      "_DynamicType second;\n"
    "} _DynamicTuple;\n"
    "_DynamicType _dt_do_add_str(_DynamicType lhs, _DynamicType rhs) {"
        "_DynamicType result;\n"
        "char *str1 = (char *) lhs.value;"
        "char *str2 = (char *) rhs.value;"
        "if (lhs.type == _DT_INT) {"
            "str1 = malloc(sizeof(char) * 11);"
            "sprintf(str1, \"%%d\", (int32_t) lhs.value);"
        "}"
        "if (rhs.type == _DT_INT) {"
            "str2 = malloc(sizeof(char) * 11);"
            "sprintf(str2, \"%%d\", (int32_t) rhs.value);"
        "}"
        "if (lhs.type == _DT_BOOL) {"
            "str1 = malloc(sizeof(char) * 6);"
            "sprintf(str1, \"%%s\", (bool) lhs.value ? \"true\" : \"false\");"
        "}"
        "if (rhs.type == _DT_BOOL) {"
            "str2 = malloc(sizeof(char) * 6);"
            "sprintf(str2, \"%%s\", (bool) rhs.value ? \"true\" : \"false\");"
        "}"
        "result.value = (size_t) malloc(strlen(str1) + strlen(str2) + 1);"
        "sprintf((char *) result.value, \"%%s%%s\", str1, str2);"
        "result.type = _DT_STR;"
        "return result;"
    "}"
    "_DynamicType _dt_do_add(_DynamicType lhs, _DynamicType rhs) {\n"
        "_DynamicType result;\n"
        "if (lhs.type != _DT_STR && rhs.type !=_DT_STR) {\n"
            "result.type = _DT_INT;\n"
            "result.value = (size_t) ((int32_t) lhs.value + (int32_t) rhs.value);\n"
        "} else {\n"
            "result = _dt_do_add_str(lhs, rhs);"
        "}\n"
        "return result;\n"
    "}\n"
    "_DynamicType _dt_do_sub(_DynamicType lhs, _DynamicType rhs) {\n"
        "_DynamicType result;\n"
        "if (lhs.type != _DT_STR && rhs.type !=_DT_STR) {\n"
            "result.type = _DT_INT;\n"
            "result.value = (size_t) ((int32_t) lhs.value - (int32_t) rhs.value);\n"
        "} else {"
            "printf(\"Sub não implementado para Str\");\n"
        "}"
        "return result;\n"
    "}\n"
    "_DynamicType _dt_do_mul(_DynamicType lhs, _DynamicType rhs) {\n"
        "_DynamicType result;\n"
        "if (lhs.type != _DT_STR && rhs.type != _DT_STR) {\n"
            "result.type = _DT_INT;\n"
            "result.value = (size_t) ((int32_t) lhs.value * (int32_t) rhs.value);\n"
        "} else {"
            "printf(\"Mul não implementado para Str\");\n"   
        "}"    
        "return result;\n"
    "}\n"
    "_DynamicType _dt_do_div(_DynamicType lhs, _DynamicType rhs) {\n"
        "_DynamicType result;\n"
        "if (lhs.type != _DT_STR && rhs.type != _DT_STR) {\n"
            "result.type = _DT_INT;\n"
            "result.value = (size_t) ((int32_t) lhs.value / (int32_t) rhs.value);\n"
        "} else {"
            "printf(\"Div não implementado para Str\");\n"   
        "}" 
        "return result;\n"
    "}\n"
    "_DynamicType _dt_do_rem(_DynamicType lhs, _DynamicType rhs) {\n"
        "_DynamicType result;\n"
        "if (lhs.type != _DT_STR && rhs.type != _DT_STR) {\n"
            "result.type = _DT_INT;\n"
            "result.value = (size_t) ((int32_t) lhs.value % (int32_t) rhs.value);\n"
        "} else {"
            "printf(\"Rem não implementado para Str\");\n"   
        "}"        
        "return result;\n"
    "}\n"
    "bool _dt_do_eq(_DynamicType lhs, _DynamicType rhs) {\n"
        "if (lhs.type != _DT_STR && rhs.type !=_DT_STR) {\n"
            "return (size_t) ((int32_t) lhs.value == (int32_t) rhs.value);\n"
        "} else if (lhs.type == _DT_STR && rhs.type == _DT_STR) {"
            "return strcmp((char *) lhs.value, (char *) rhs.value) == 0;"
        "}"
        "return false;"
    "}\n"
    "bool _dt_do_neq(_DynamicType lhs, _DynamicType rhs) {\n"
        "if (lhs.type != _DT_STR && rhs.type !=_DT_STR) {\n"
            "return (size_t) ((int32_t) lhs.value != (int32_t) rhs.value);\n"
        "} else if (lhs.type == _DT_STR && rhs.type == _DT_STR) {"
            "return strcmp((char *) lhs.value, (char *) rhs.value) != 0;"
        "}"
        "return false;"
    "}\n"
    "bool _dt_do_lt(_DynamicType lhs, _DynamicType rhs) {\n"
        "if (lhs.type != _DT_STR && rhs.type !=_DT_STR) {\n"
            "return (size_t) ((int32_t) lhs.value < (int32_t) rhs.value);\n"
        "} else if (lhs.type == _DT_STR && rhs.type == _DT_STR) {"
            "return strcmp((char *) lhs.value, (char *) rhs.value) < 0;"
        "}"
        "return false;"
    "}\n"
    "bool _dt_do_gt(_DynamicType lhs, _DynamicType rhs) {\n"
        "if (lhs.type != _DT_STR && rhs.type !=_DT_STR) {\n"
            "return (size_t) ((int32_t) lhs.value > (int32_t) rhs.value);\n"
        "} else if (lhs.type == _DT_STR && rhs.type == _DT_STR) {"
            "return strcmp((char *) lhs.value, (char *) rhs.value) > 0;"
        "}"
        "return false;"
    "}\n"
    "bool _dt_do_lte(_DynamicType lhs, _DynamicType rhs) {\n"
        "if (lhs.type != _DT_STR && rhs.type !=_DT_STR) {\n"
            "return (size_t) ((int32_t) lhs.value <= (int32_t) rhs.value);\n"
        "} else if (lhs.type == _DT_STR && rhs.type == _DT_STR) {"
            "return strcmp((char *) lhs.value, (char *) rhs.value) <= 0;"
        "}"
        "return false;"
    "}\n"
    "bool _dt_do_gte(_DynamicType lhs, _DynamicType rhs) {\n"
        "if (lhs.type != _DT_STR && rhs.type !=_DT_STR) {\n"
            "return (size_t) ((int32_t) lhs.value >= (int32_t) rhs.value);\n"
        "} else if (lhs.type == _DT_STR && rhs.type == _DT_STR) {"
            "return strcmp((char *) lhs.value, (char *) rhs.value) >= 0;"
        "}"
        "return false;"
    "}\n"
    "bool _dt_do_and(_DynamicType lhs, _DynamicType rhs) {\n"
        "if (lhs.type != _DT_STR && rhs.type !=_DT_STR) {\n"
            "return (size_t) ((int32_t) lhs.value && (int32_t) rhs.value);\n"
        "}\n" 
        "printf(\"And não implementado para Str\");\n"
        "return false;\n"
    "}\n"
    "bool _dt_do_or(_DynamicType lhs, _DynamicType rhs) {\n"
        "if (lhs.type != _DT_STR && rhs.type !=_DT_STR) {\n"
            "return (size_t) ((int32_t) lhs.value || (int32_t) rhs.value);\n"
        "}\n" 
        "printf(\"Or não implementado para Str\");\n"
        "return false;\n"
    "}\n"
    "size_t _dt_len(_DynamicType *dt) {"
        "if (dt->type == _DT_STR) {"
            "return strlen((char *) dt->value);"
        "} else if (dt->type == _DT_BOOL) {"
            "return 6;"
        "} else if (dt->type == _DT_INT) {"
            "return 11;"
        "}"
        "return _dt_len((_DynamicType *) &((_DynamicTuple *) dt->value)->first) + _dt_len((_DynamicType *) &((_DynamicTuple *) dt->value)->second) + 4;"
    "}"
    "char *_dt_do_sprintf(_DynamicType *dt) {\n"
        "char *ret;"
        "size_t len = _dt_len(dt);"
        "ret = malloc(sizeof(char) * len + 1);"
        "if (dt->type == _DT_INT) {\n"
            "sprintf(ret, \"%%d\", (int32_t) dt->value);\n"
        "} else if (dt->type == _DT_BOOL) {\n"
            "sprintf(ret, \"%%s\", (bool) dt->value ? \"true\" : \"false\");\n"
        "} else if (dt->type == _DT_TUPLE) {\n"
            "_DynamicTuple *tuple = (_DynamicTuple *) dt->value;"
            "sprintf(ret, \"(%%s, %%s)\", _dt_do_sprintf(&tuple->first), _dt_do_sprintf(&tuple->second));\n"     
        "} else {\n"
            "sprintf(ret, \"%%s\", (char *) dt->value);\n"
        "}\n"
        "return ret;"
    "}\n"    
    "_DynamicType _dt_do_print(_DynamicType dt) {\n"
        "char *str = _dt_do_sprintf(&dt);"
        "printf(\"%%s\\n\", str);\n"
        "return dt;"
    "}\n"    
    "_DynamicType _dt_new_int(int32_t value) {\n"
        "_DynamicType dt;\n"
        "dt.type = _DT_INT;\n"
        "dt.value = (size_t) value;\n"
        "return dt;\n"
    "}\n"
    "_DynamicType _dt_new_str(char *value) {\n"
        "_DynamicType dt;\n"
        "dt.type = _DT_STR;\n"
        "dt.value = (size_t) value;\n"
        "return dt;\n"
    "}\n"
    "_DynamicType _dt_new_bool(bool value) {\n"
        "_DynamicType dt;\n"
        "dt.type = _DT_BOOL;\n"
        "dt.value = (size_t) value;\n"
        "return dt;\n"
    "}\n"
    "_DynamicType _dt_new_tuple(_DynamicType first, _DynamicType second) {\n"
        "_DynamicType dt;\n"
        "_DynamicTuple *tuple;\n"
        "tuple = (_DynamicTuple *) malloc(sizeof(_DynamicTuple));\n"
        "tuple->first = first;\n"
        "tuple->second = second;\n"
        "dt.type = _DT_TUPLE;\n"
        "dt.value = (size_t) tuple;\n"
        "return dt;\n"
    "}\n"
    "_DynamicType _dt_get_tuple_first(_DynamicType tuple) {\n"
        "_DynamicType dt;"
        "if (tuple.type == _DT_TUPLE) {"
            "dt = ((_DynamicTuple *) tuple.value)->first;"
        "} else {"
            "printf(\"Variável não é uma tupla\\n\");"
        "}"
        "return dt;"
    "}\n"
    "_DynamicType _dt_get_tuple_second(_DynamicType tuple) {\n"
        "_DynamicType dt;"
        "if (tuple.type == _DT_TUPLE) {"
            "dt = ((_DynamicTuple *) tuple.value)->second;"
        "} else {"
            "printf(\"Variável não é uma tupla\\n\");"
        "}"
        "return dt;"
    "}\n"
    "%s\n"
    "int main(int argc, char **argv) {\n"
      "%s;\n"
    "return 0;\n"
    "}\n";