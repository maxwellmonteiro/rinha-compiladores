#include "term.h"
#include "../util/dynamic_type.h"
#include <inttypes.h>
#include <stdbool.h>

typedef _DynamicType (*binary_func_t)(_DynamicType, _DynamicType);

extern uint16_t binary_term_get_op(const char *op_str);
extern binary_func_t binary_term_get_op_func(uint16_t op);