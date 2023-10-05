#include "binding_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void push(BindingList *self, Binding *value);
static Binding *pop(BindingList *self);
static Binding *find(BindingList *self, const char *name);
static bool is_full(BindingList *self);
static void destroy(BindingList *self);

Binding *binding_new(const char *name, _DynamicType value) {
    Binding *binding = malloc(sizeof(Binding));   
    if (name != NULL) {
        binding->name = malloc((sizeof(char) * strlen(name)) + 1);
        strcpy(binding->name, name);
    } else {
        binding->name = NULL;
    }
    binding->value = value;
    return binding;
}

void binding_set_name(Binding *binding, const char *name) {
    binding->name = malloc((sizeof(char) * strlen(name)) + 1);
    strcpy(binding->name, name);    
}

BindingList *binding_list_new(size_t size) {
    BindingList *self = malloc(sizeof(BindingList));
    self->values = malloc(sizeof(Binding *) * size);
    self->size = 0;
    self->max_size = size;
    self->push = push;
    self->pop = pop;
    self->find = find;
    self->is_full = is_full;
    self->destroy = destroy;
    return self;
}

void push(BindingList *self, Binding *value) {
    if (self->size >= self->max_size) {
        printf("Tamanho lista excedido");
        exit(EXIT_FAILURE);
    }
    self->values[self->size] = value;
    self->size++;
}

Binding *pop(BindingList *self) {
    Binding *val = self->values[0];
    self->size--;
    memcpy(&self->values[0], &self->values[1], sizeof(Binding *) * self->size);
    return val;
}

Binding *find(BindingList *self, const char *name) {
    for (int i = self->size - 1; i >= 0; i--) {
        if (self->values[i]->name != NULL && strcmp(self->values[i]->name, name) == 0) {
            return self->values[i];
        }
    }
    return NULL;
}

bool is_full(BindingList *self) {
    return self->size >= self->max_size;
}

void destroy(BindingList *self) {
    free(self->values);
    self->values = NULL;
    free(self);
}

