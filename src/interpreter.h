#include "util/dynamic_type.h"
#include "util/closure.h"
#include <inttypes.h>
#include <jansson.h>

extern _DynamicType interpreter_parse(json_t *expression, Closure *closure);