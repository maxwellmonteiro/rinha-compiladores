#include "term.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char *TERM_STRING[] = {
    FOREACH_TERM(GENERATE_STRING)
    TERM_NULL
};

static void destroy(Term *self) {
    free(self);
}

Term *term_new(uint16_t kind) {
    Term *self = (Term *) malloc(sizeof(Term));

    self->kind = kind;

    self->destroy = destroy;

    return self;
}

uint16_t term_get_kind(const char *kind_str) {
    for (int i = 0; strcmp(TERM_STRING[i], TERM_NULL) != 0; i++) {
        if (strcmp(TERM_STRING[i], kind_str) == 0) {
            return i;
        }
    }
    return TERM_NOT_FOUND;
}

size_t _dt_len(_DynamicType *dt) {
    if (dt->type == _DT_STR)
    {
        return strlen((char *)dt->value);
    }
    else if (dt->type == _DT_BOOL)
    {
        return 6;
    }
    else if (dt->type == _DT_INT)
    {
        return 11;
    }
    return _dt_len((_DynamicType *)&((_DynamicTuple *)dt->value)->first) + _dt_len((_DynamicType *)&((_DynamicTuple *)dt->value)->second) + 4;
}

char *_dt_do_sprintf(_DynamicType *dt) {
    char *ret;
    size_t len = _dt_len(dt);
    ret = malloc(sizeof(char) * len + 1);
    if (dt->type == _DT_INT) {
        sprintf(ret, "%d", (int32_t)dt->value);
    }
    else if (dt->type == _DT_BOOL) {
        sprintf(ret, "%s", (bool) dt->value ? "true" : "false");
    }
    else if (dt->type == _DT_TUPLE) {
        _DynamicTuple *tuple = (_DynamicTuple *)dt->value;
        sprintf(ret, "(%s, %s)", _dt_do_sprintf(&tuple->first), _dt_do_sprintf(&tuple->second));
    }
    else {
        sprintf(ret, "%s", (char *) dt->value);
    }
    return ret;
}

_DynamicType _dt_do_print(_DynamicType dt) {
    char *str = _dt_do_sprintf(&dt);
    printf("%s\n", str);
    fflush(stdout);
    return dt;
}

_DynamicType _dt_new_int(int32_t value) {
    _DynamicType dt;
    dt.type = _DT_INT;
    dt.value = (size_t)value;
    return dt;
}

_DynamicType _dt_new_str(const char *value) {
    _DynamicType dt;
    dt.type = _DT_STR;
    dt.value = (size_t) value;
    return dt;
}

_DynamicType _dt_new_bool(bool value) {
    _DynamicType dt;
    dt.type = _DT_BOOL;
    dt.value = (size_t) value;
    return dt;
}

_DynamicType _dt_new_tuple(_DynamicType first, _DynamicType second) {
    _DynamicType dt;
    _DynamicTuple *tuple;
    tuple = (_DynamicTuple *) malloc(sizeof(_DynamicTuple));
    tuple->first = first;
    tuple->second = second;
    dt.type = _DT_TUPLE;
    dt.value = (size_t) tuple;
    return dt;
}

_DynamicType _dt_get_tuple_first(_DynamicType tuple) {
    _DynamicType dt;
    if (tuple.type == _DT_TUPLE) {
        dt = ((_DynamicTuple *) tuple.value)->first;
    }
    else {
        printf("Variável não é uma tupla\n");
    }
    return dt;
}

_DynamicType _dt_get_tuple_second(_DynamicType tuple) {
    _DynamicType dt;
    if (tuple.type == _DT_TUPLE) {
        dt = ((_DynamicTuple *) tuple.value)->second;
    }
    else {
        printf("Variável não é uma tupla\n");
        dt = tuple;
    }
    return dt;
}