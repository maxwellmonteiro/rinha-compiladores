#include "fn_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void push(FnList *self, Fn *value);
static Fn *pop(FnList *self);
static Fn *find(FnList *self, const char *src);
static bool is_full(FnList *self);
static void destroy(FnList *self);

Fn *fn_new(const char *name, json_t *src) {
    Fn* fn = malloc(sizeof(Fn));
    strncpy(fn->name, name, FUNCTION_MAX_NAME - 1);
    fn->src = src;
    return fn;
}

FnList *fn_list_new(size_t size) {
    FnList *self = malloc(sizeof(FnList));
    self->values = malloc(sizeof(Fn *) * size);
    self->size = 0;
    self->max_size = size;
    self->push = push;
    self->pop = pop;
    self->find = find;
    self->is_full = is_full;
    self->destroy = destroy;
    return self;
}

void push(FnList *self, Fn *value) {
    if (self->size >= self->max_size) {
        printf("Tamanho lista excedido");
        exit(EXIT_FAILURE);
    }
    self->values[self->size] = value;
    self->size++;
}

Fn *pop(FnList *self) {
    Fn *val = self->values[0];
    self->size--;
    memcpy(&self->values[0], &self->values[1], sizeof(Fn *) * self->size);
    return val;
}

Fn *find(FnList *self, const char *name) {
    for (int i = 0; i < self->size; i++) {
        if (strcmp(self->values[i]->name, name) == 0) {
            return self->values[i];
        }
    }
    return NULL;
}

bool is_full(FnList *self) {
    return self->size >= self->max_size;
}

void destroy(FnList *self) {
    free(self->values);
    self->values = NULL;
    free(self);
}

