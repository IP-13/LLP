#pragma once

#include <inttypes.h>
#include <string.h>

#include "tuple.h"


enum filter_type {
    BOOL_EQ,
    BOOL_NEQ,
    INT_EQ,
    INT_NEQ,
    FLOAT_EQ,
    FLOAT_NEQ,
    STR_EQ,
    STR_NEQ,
    INT_GR,
    INT_LESS,
    INT_GR_EQ,
    INT_LESS_EQ,
    FLOAT_GR,
    FLOAT_LESS,
    FLOAT_GR_EQ,
    FLOAT_LESS_EQ,
    STR_LIKE
};

struct filter {
    uint16_t attribute_num; // number of attribute in tuple ot which filter_type is applied
    enum filter_type filter_type;
    void *value;
};

int is_match(struct tuple *tuple, struct filter *filter);

