#include "var_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void push(VarList *self, Variable *value);
static Variable *pop(VarList *self);
static Variable *find(VarList *self, const char *src);
static bool is_full(VarList *self);
static void destroy(VarList *self);

Variable *variable_new() {
    Variable* Variable = malloc(sizeof(Variable));
    Variable->name[0] = 0;
    return Variable;
}

VarList *var_list_new(size_t size) {
    VarList *self = malloc(sizeof(VarList));
    self->values = malloc(sizeof(Variable *) * size);
    self->size = 0;
    self->max_size = size;
    self->push = push;
    self->pop = pop;
    self->find = find;
    self->is_full = is_full;
    self->destroy = destroy;
    return self;
}

void push(VarList *self, Variable *value) {
    if (self->size >= self->max_size) {
        printf("Tamanho lista excedido");
        exit(EXIT_FAILURE);
    }
    self->values[self->size] = value;
    self->size++;
}

Variable *pop(VarList *self) {
    Variable *val = self->values[0];
    self->size--;
    memcpy(&self->values[0], &self->values[1], sizeof(Variable *) * self->size);
    return val;
}

Variable *find(VarList *self, const char *name) {
    for (int i = 0; i < self->size; i++) {
        if (strcmp(self->values[i]->name, name) == 0) {
            return self->values[i];
        }
    }
    return NULL;
}

bool is_full(VarList *self) {
    return self->size >= self->max_size;
}

void destroy(VarList *self) {
    free(self->values);
    self->values = NULL;
    free(self);
}

