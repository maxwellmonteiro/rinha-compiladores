#include "binary_term.h"
#include <stdlib.h>
#include <string.h>

static const char *BINARY_STRING[] = {
    FOREACH_BINARY(GENERATE_STRING)
    BINARY_NULL
};

static const char *BINARY_LOGIC_STRING[] = {
    FOREACH_BINARY_LOGIC(GENERATE_STRING)
    BINARY_NULL
};

static const char *BINARY_TARGET[] = {
    FOREACH_BINARY_TARGET(GENERATE_STRING)
    BINARY_NULL
};

static void destroy(BinaryTerm *self) {
    free(self);
}

BinaryTerm *binary_term_new() {
    BinaryTerm *self = (BinaryTerm *) malloc(sizeof(BinaryTerm));

    self->destroy = destroy;

    return self;
}

uint16_t binary_term_get_op(const char *op_str) {
    for (int i = 0; strcmp(BINARY_STRING[i], BINARY_NULL) != 0; i++) {
        if (strcmp(BINARY_STRING[i], op_str) == 0) {
            return i;
        }
    }
    return BINARY_NOT_FOUND;
}

bool binary_term_is_logic_op(const char *op_str) {
    for (int i = 0; strcmp(BINARY_LOGIC_STRING[i], BINARY_NULL) != 0; i++) {
        if (strcmp(BINARY_LOGIC_STRING[i], op_str) == 0) {
            return true;
        }
    }
    return false;
}

const char *binary_term_get_op_target(uint16_t op) {
    return BINARY_TARGET[op];
}