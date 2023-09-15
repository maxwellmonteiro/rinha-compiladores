#include <inttypes.h>
#include <stdbool.h>

#ifndef CALLEE_H
#define CALLEE_H


#define CALLEE_MAX_NAME 128
#define CALLEE_MAX_ARGS 512

typedef struct Callee {
    char name[CALLEE_MAX_NAME];
    char args[CALLEE_MAX_ARGS];
} Callee;


typedef struct CalleeList {
    int max_size;
    int size;
    Callee **values;
    void (*push) (struct CalleeList *, Callee *);
    Callee *(*pop) (struct CalleeList *);
    Callee *(*find) (struct CalleeList *, char *);
    bool (*is_full) (struct CalleeList *);
    void (*destroy) (struct CalleeList *);
} CalleeList;

extern CalleeList *callee_list_new();
extern Callee *callee_new();

#endif