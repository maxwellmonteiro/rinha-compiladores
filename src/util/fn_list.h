#include <inttypes.h>
#include <stdbool.h>

#ifndef FN_LIST_H
#define FN_LIST_H

#define FUNCTION_MAX_NAME 128
#define FUNCTION_MAX_COUNT 32
#define FUNCTION_SRC_MAX_LEN 1024
#define FUNCTION_MAX_ARGS 16

typedef struct Fn {
    char name[FUNCTION_MAX_NAME];
    char src[FUNCTION_SRC_MAX_LEN];
} Fn;

typedef struct FnList {
    int max_size;
    int size;
    Fn **values;
    void (*push) (struct FnList *, Fn *);
    Fn *(*pop) (struct FnList *);
    Fn *(*find) (struct FnList *, char *);
    bool (*is_full) (struct FnList *);
    void (*destroy) (struct FnList *);
} FnList;

extern FnList *fn_list_new();
extern Fn *fn_new();

#endif