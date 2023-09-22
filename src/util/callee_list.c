#include "callee_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void push(CalleeList *self, Callee *value);
static Callee *pop(CalleeList *self);
static Callee *find(CalleeList *self, char *src);
static bool is_full(CalleeList *self);
static void destroy(CalleeList *self);

Callee *callee_new() {
    Callee *callee = malloc(sizeof(Callee));
    callee->name[0] = 0;
    callee->args[0] = 0;
    return callee;
}

CalleeList *callee_list_new(size_t size) {
    CalleeList *self = malloc(sizeof(CalleeList));
    self->values = malloc(sizeof(Callee *) * size);
    self->size = 0;
    self->max_size = size;
    self->push = push;
    self->pop = pop;
    self->find = find;
    self->is_full = is_full;
    self->destroy = destroy;
    return self;
}

void push(CalleeList *self, Callee *value) {
    if (self->size >= self->max_size) {
        printf("Tamanho lista excedido");
        exit(EXIT_FAILURE);
    }
    self->values[self->size] = value;
    self->size++;
}

Callee *pop(CalleeList *self) {
    Callee *val = self->values[0];
    self->size--;
    memcpy(&self->values[0], &self->values[1], sizeof(Callee *) * self->size);
    return val;
}

Callee *find(CalleeList *self, char *name) {
    for (int i = 0; i < self->size; i++) {
        if (strcmp(self->values[i]->name, name) == 0) {
            return self->values[i];
        }
    }
    return NULL;
}

bool is_full(CalleeList *self) {
    return self->size >= self->max_size;
}

void destroy(CalleeList *self) {
    free(self->values);
    self->values = NULL;
    free(self);
}

