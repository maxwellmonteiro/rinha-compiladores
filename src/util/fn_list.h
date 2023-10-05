#include <inttypes.h>
#include <stdbool.h>
#include <jansson.h>

#ifndef FN_LIST_H
#define FN_LIST_H

#define FUNCTION_MAX_NAME 1024
#define FUNCTION_MAX_COUNT 64
#define FUNCTION_SRC_MAX_LEN 102400
#define FUNCTION_MAX_ARGS 64

typedef struct Fn {
    char name[FUNCTION_MAX_NAME];
    json_t *src;
} Fn;

typedef struct FnList {
    int max_size;
    int size;
    Fn **values;
    void (*push) (struct FnList *, Fn *);
    Fn *(*pop) (struct FnList *);
    Fn *(*find) (struct FnList *, const char *);
    bool (*is_full) (struct FnList *);
    void (*destroy) (struct FnList *);
} FnList;

extern FnList *fn_list_new();
extern Fn *fn_new(const char *name, json_t *src);

#endif