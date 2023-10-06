#include "dynamic_type.h"
#include <stdbool.h>

#ifndef BINDING_H
#define BINDING_H

#define BINDING_LIST_INITIAL_SIZE 128

typedef struct Binding {
    char *name;
    _DynamicType value;
} Binding;

typedef struct BindingList {
    int max_size;
    int size;
    Binding **values;
    void (*push) (struct BindingList *, Binding *);
    Binding *(*pop) (struct BindingList *);
    Binding *(*find) (struct BindingList *, const char *);
    bool (*is_full) (struct BindingList *);
    void (*destroy) (struct BindingList *);
} BindingList;

extern BindingList *binding_list_new(size_t initial_size);
extern Binding *binding_new(const char *name, _DynamicType value);
extern void binding_set_name(Binding *binding, const char *name);

#endif