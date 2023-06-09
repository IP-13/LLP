#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "my_malloc.h"
#include "data_type.h"
#include "tuple.h"
#include "filters.h"


void print_tuple(struct tuple *tuple, uint32_t num_of_attributes, const enum data_type *table_scheme);


void free_filter_value(void *value, enum filter_cond filter_cond, enum filter_type filter_type);


void free_filters(uint16_t num_of_filters, struct filter **filters);

