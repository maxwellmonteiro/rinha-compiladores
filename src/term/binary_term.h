#include <inttypes.h>
#include <stdbool.h>
#include "term.h"

#define BINARY_NULL "BINARY_NULL"
#define BINARY_NOT_FOUND 9999

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

enum BINARY_ENUM {
    FOREACH_BINARY(GENERATE_ENUM)
};

typedef struct BinaryTerm {
    Term *lhs;
    uint16_t op;
    Term *rhs;
    void (*destroy)(struct BinaryTerm *);
} BinaryTerm;

extern BinaryTerm *binary_term_new();
extern uint16_t binary_term_get_op(const char *op_str);
extern const char *binary_term_get_op_target(uint16_t op);
extern bool binary_term_is_logic_op(const char *op_str);