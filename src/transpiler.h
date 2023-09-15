#include "util/context.h"
#include <inttypes.h>
#include <jansson.h>

#ifndef INTERPRETER_H
#define INTERPRETER_H

// Intermediate Representation in C code
typedef struct IR {
    char *exp;
    uint8_t type;
} IR;

enum IR_TYPE {
    IR_INT,
    IR_STR,
    IR_BOOL,
    IR_TUPLE,
    IR_FUNCTION,
    IR_DT,
    IR_UNDEFINED
};

extern void transpiler_free_ir(IR *ir);
extern IR *transpiler_parse(json_t *expression, Context *context);

#endif