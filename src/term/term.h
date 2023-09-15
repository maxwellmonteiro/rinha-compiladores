#include <inttypes.h>

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

typedef struct Term {
    uint16_t kind;
    void *term;
    void (*destroy)(struct Term *);
} Term;

extern Term *term_new(uint16_t kind);
extern uint16_t term_get_kind(const char *kind_str);

#endif