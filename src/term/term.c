#include "term.h"
#include <stdlib.h>
#include <string.h>

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