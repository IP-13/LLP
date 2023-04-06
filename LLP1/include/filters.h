#pragma once

#include <string.h>
#include <inttypes.h>
#include "tuple.h"
#include "data_type.h"
#include "column.h"


enum filter_type {
    CONST,
    REFERENCE
};


enum filter_cond {
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
    char *col_name;
    enum filter_type filter_type;
    enum filter_cond filter_cond;
    void *value; // if filter type CONST contains const_value, if filter type REFERENCE contains column name
};


int is_match(struct tuple *tuple, struct filter *filter, uint64_t num_of_columns, struct column **columns);

