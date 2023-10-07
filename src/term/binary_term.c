#include "binary_term.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BINARY_NULL "BINARY_NULL"
#define BINARY_NOT_FOUND 9999

_DynamicType _dt_do_add_str(_DynamicType lhs, _DynamicType rhs);
_DynamicType _dt_do_add(_DynamicType lhs, _DynamicType rhs);
_DynamicType _dt_do_sub(_DynamicType lhs, _DynamicType rhs);
_DynamicType _dt_do_mul(_DynamicType lhs, _DynamicType rhs);
_DynamicType _dt_do_div(_DynamicType lhs, _DynamicType rhs);
_DynamicType _dt_do_rem(_DynamicType lhs, _DynamicType rhs);
_DynamicType _dt_do_eq(_DynamicType lhs, _DynamicType rhs);
_DynamicType _dt_do_neq(_DynamicType lhs, _DynamicType rhs);
_DynamicType _dt_do_lt(_DynamicType lhs, _DynamicType rhs);
_DynamicType _dt_do_gt(_DynamicType lhs, _DynamicType rhs);
_DynamicType _dt_do_lte(_DynamicType lhs, _DynamicType rhs);
_DynamicType _dt_do_gte(_DynamicType lhs, _DynamicType rhs);
_DynamicType _dt_do_and(_DynamicType lhs, _DynamicType rhs);
_DynamicType _dt_do_or(_DynamicType lhs, _DynamicType rhs);

#define FOREACH_BINARY_LOGIC(BINARY) \
        BINARY(Eq) \
        BINARY(Neq) \
        BINARY(Lt) \
        BINARY(Gt) \
        BINARY(Lte) \
        BINARY(Gte) \
        BINARY(And) \
        BINARY(Or)

#define FOREACH_BINARY(BINARY) \
        BINARY(Add) \
        BINARY(Sub) \
        BINARY(Mul) \
        BINARY(Div) \
        BINARY(Rem) \
        BINARY(Eq) \
        BINARY(Neq) \
        BINARY(Lt) \
        BINARY(Gt) \
        BINARY(Lte) \
        BINARY(Gte) \
        BINARY(And) \
        BINARY(Or)

#define FOREACH_BINARY_TARGET(BINARY) \
        BINARY(_dt_do_add) \
        BINARY(_dt_do_sub) \
        BINARY(_dt_do_mul) \
        BINARY(_dt_do_div) \
        BINARY(_dt_do_rem) \
        BINARY(_dt_do_eq) \
        BINARY(_dt_do_neq) \
        BINARY(_dt_do_lt) \
        BINARY(_dt_do_gt) \
        BINARY(_dt_do_lte) \
        BINARY(_dt_do_gte) \
        BINARY(_dt_do_and) \
        BINARY(_dt_do_or)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define GENERATE_FUNC(STRING) STRING,

enum BINARY_ENUM {
    FOREACH_BINARY(GENERATE_ENUM)
};

static const char *BINARY_STRING[] = {
    FOREACH_BINARY(GENERATE_STRING)
    BINARY_NULL
};

static binary_func_t BINARY_FUNC[] = {
    FOREACH_BINARY_TARGET(GENERATE_FUNC)
    NULL
};

_DynamicType _dt_do_add_str(_DynamicType lhs, _DynamicType rhs) {
    _DynamicType result;
    char *str1 = (char *) lhs.value;
    char *str2 = (char *) rhs.value;
    if (lhs.type == _DT_INT) {
        str1 = malloc(sizeof(char) * 11);
        sprintf(str1, "%d", (int32_t) lhs.value);
    }
    if (rhs.type == _DT_INT) {
        str2 = malloc(sizeof(char) * 11);
        sprintf(str2, "%d", (int32_t) rhs.value);
    }
    if (lhs.type == _DT_BOOL) {
        str1 = malloc(sizeof(char) * 6);
        sprintf(str1, "%s", (bool) lhs.value ? "true" : "false");
    }
    if (rhs.type == _DT_BOOL) {
        str2 = malloc(sizeof(char) * 6);
        sprintf(str2, "%s", (bool) rhs.value ? "true" : "false");
    }
    result.value = (size_t) malloc(strlen(str1) + strlen(str2) + 1);
    sprintf((char *) result.value, "%s%s", str1, str2);
    result.type = _DT_STR;
    return result;
}

_DynamicType _dt_do_add(_DynamicType lhs, _DynamicType rhs) {
    _DynamicType result;
    if (lhs.type != _DT_STR && rhs.type != _DT_STR) {
        result.type = _DT_INT;
        result.value = (size_t) ((int32_t) lhs.value + (int32_t) rhs.value);
    }
    else {
        result = _dt_do_add_str(lhs, rhs);
    }
    return result;
}

_DynamicType _dt_do_sub(_DynamicType lhs, _DynamicType rhs) {
    _DynamicType result;
    if (lhs.type != _DT_STR && rhs.type != _DT_STR) {
        result.type = _DT_INT;
        result.value = (size_t) ((int32_t) lhs.value - (int32_t) rhs.value);
    }
    else {
        printf("Sub não implementado para Str");
    }
    return result;
}

_DynamicType _dt_do_mul(_DynamicType lhs, _DynamicType rhs) {
    _DynamicType result;
    if (lhs.type != _DT_STR && rhs.type != _DT_STR) {
        result.type = _DT_INT;
        result.value = (size_t) ((int32_t) lhs.value * (int32_t) rhs.value);
    }
    else {
        printf("Mul não implementado para Str");
    }
    return result;
}

_DynamicType _dt_do_div(_DynamicType lhs, _DynamicType rhs) {
    _DynamicType result;
    if (lhs.type != _DT_STR && rhs.type != _DT_STR) {
        result.type = _DT_INT;
        result.value = (size_t) ((int32_t) lhs.value / (int32_t) rhs.value);
    }
    else {
        printf("Div não implementado para Str");
    }
    return result;
}

_DynamicType _dt_do_rem(_DynamicType lhs, _DynamicType rhs) {
    _DynamicType result;
    if (lhs.type != _DT_STR && rhs.type != _DT_STR) {
        result.type = _DT_INT;
        result.value = (size_t) ((int32_t) lhs.value % (int32_t) rhs.value);
    }
    else {
        printf("Rem não implementado para Str");
    }
    return result;
}

_DynamicType _dt_do_eq(_DynamicType lhs, _DynamicType rhs) {
    _DynamicType result = { _DT_BOOL, (size_t) false };
    if (lhs.type != _DT_STR && rhs.type != _DT_STR) {
        result.value = (size_t) ((int32_t) lhs.value == (int32_t) rhs.value);
    }
    else if (lhs.type == _DT_STR && rhs.type == _DT_STR) {
        result.value = strcmp((char *) lhs.value, (char *) rhs.value) == 0;
    }
    return result;
}

_DynamicType _dt_do_neq(_DynamicType lhs, _DynamicType rhs) {
    _DynamicType result = { _DT_BOOL, (size_t) false };
    if (lhs.type != _DT_STR && rhs.type != _DT_STR) {
        result.value = (size_t) ((int32_t) lhs.value != (int32_t) rhs.value);
    }
    else if (lhs.type == _DT_STR && rhs.type == _DT_STR) {
        result.value = strcmp((char *) lhs.value, (char *) rhs.value) != 0;
    }
    return result;
}

_DynamicType _dt_do_lt(_DynamicType lhs, _DynamicType rhs) {
    _DynamicType result = { _DT_BOOL, (size_t) false };
    if (lhs.type != _DT_STR && rhs.type != _DT_STR) {
        result.value = (size_t) ((int32_t) lhs.value < (int32_t) rhs.value);
    }
    else if (lhs.type == _DT_STR && rhs.type == _DT_STR) {
        result.value = strcmp((char *) lhs.value, (char *) rhs.value) < 0;
    }
    return result;
}

_DynamicType _dt_do_gt(_DynamicType lhs, _DynamicType rhs) {
    _DynamicType result = { _DT_BOOL, (size_t) false };
    if (lhs.type != _DT_STR && rhs.type != _DT_STR) {
        result.value = (size_t) ((int32_t) lhs.value > (int32_t) rhs.value);
    }
    else if (lhs.type == _DT_STR && rhs.type == _DT_STR) {
        result.value = strcmp((char *) lhs.value, (char *) rhs.value) > 0;
    }
    return result;
}

_DynamicType _dt_do_lte(_DynamicType lhs, _DynamicType rhs) {
    _DynamicType result = { _DT_BOOL, (size_t) false };
    if (lhs.type != _DT_STR && rhs.type != _DT_STR) {
        result.value = (size_t) ((int32_t) lhs.value <= (int32_t) rhs.value);
    }
    else if (lhs.type == _DT_STR && rhs.type == _DT_STR) {
        result.value = strcmp((char *) lhs.value, (char *) rhs.value) <= 0;
    }
    return result;
}

_DynamicType _dt_do_gte(_DynamicType lhs, _DynamicType rhs) {
    _DynamicType result = { _DT_BOOL, (size_t) false };
    if (lhs.type != _DT_STR && rhs.type != _DT_STR) {
        result.value = (size_t) ((int32_t) lhs.value >= (int32_t) rhs.value);
    }
    else if (lhs.type == _DT_STR && rhs.type == _DT_STR) {
        result.value = strcmp((char *) lhs.value, (char *) rhs.value) >= 0;
    }
    return result;
}

_DynamicType _dt_do_and(_DynamicType lhs, _DynamicType rhs) {
    _DynamicType result = { _DT_BOOL, (size_t) false };
    if (lhs.type != _DT_STR && rhs.type != _DT_STR) {
        result.value = (size_t) ((int32_t) lhs.value && (int32_t) rhs.value);
    } else {
        printf("And não implementado para Str");
    }
    return result;
}

_DynamicType _dt_do_or(_DynamicType lhs, _DynamicType rhs) {
    _DynamicType result = { _DT_BOOL, (size_t) false };
    if (lhs.type != _DT_STR && rhs.type != _DT_STR)
    {
        result.value = (size_t) ((int32_t) lhs.value || (int32_t) rhs.value);
    } else {
        printf("Or não implementado para Str");
    }
    return result;
}

uint16_t binary_term_get_op(const char *op_str) {
    for (int i = 0; strcmp(BINARY_STRING[i], BINARY_NULL) != 0; i++) {
        if (strcmp(BINARY_STRING[i], op_str) == 0) {
            return i;
        }
    }
    return BINARY_NOT_FOUND;
}

inline binary_func_t binary_term_get_op_func(uint16_t op) {
    return BINARY_FUNC[op];
}