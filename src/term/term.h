#include "../util/dynamic_type.h"
#include <inttypes.h>
#include <stdbool.h>

#ifndef TERM_H
#define TERM_H

#define TERM_NULL "TERM_NULL"
#define TERM_NOT_FOUND 9999

#define FOREACH_TERM(TERM) \
        TERM(Binary) \
        TERM(Int) \
        TERM(Str) \
        TERM(If) \
        TERM(Bool) \
        TERM(Function) \
        TERM(Call) \
        TERM(Let) \
        TERM(Var) \
        TERM(Print) \
        TERM(Tuple) \
        TERM(First) \
        TERM(Second)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum TERM_ENUM {
    FOREACH_TERM(GENERATE_ENUM)
};

extern uint16_t term_get_kind(const char *kind_str);
extern _DynamicType _dt_do_print(_DynamicType dt);
extern _DynamicType _dt_new_tuple(_DynamicType first, _DynamicType second);
extern _DynamicType _dt_get_tuple_first(_DynamicType tuple);
extern _DynamicType _dt_get_tuple_second(_DynamicType tuple);
extern _DynamicType _dt_new_int(int32_t value);
extern _DynamicType _dt_new_str(const char *value);
extern _DynamicType _dt_new_bool(bool value);

#endif