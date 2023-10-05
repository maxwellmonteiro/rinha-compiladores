#include "dynamic_type.h"
#include <inttypes.h>
#include <stdbool.h>

#ifndef VAR_LIST_H
#define VAR_LIST_H

#define VARIABLE_MAX_NAME 64

typedef struct Variable {
    char name[VARIABLE_MAX_NAME];
    _DynamicType value;
} Variable;

typedef struct VarList {
    int max_size;
    int size;
    Variable **values;
    void (*push) (struct VarList *, Variable *);
    Variable *(*pop) (struct VarList *);
    Variable *(*find) (struct VarList *, const char *);
    bool (*is_full) (struct VarList *);
    void (*destroy) (struct VarList *);
} VarList;

extern VarList *var_list_new(size_t size);
extern Variable *variable_new();

#endif