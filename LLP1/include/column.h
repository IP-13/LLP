#pragma once

#include <inttypes.h>
#include <string.h>
#include "data_type.h"
#include "my_malloc.h"

struct column {
    enum data_type data_type; // in file
    uint64_t name_size; // in file
    char *name; // in file
};


struct column *create_column(enum data_type data_type, uint64_t name_size, char *name);


void free_column(struct column *column);


int64_t get_column_index(char *name, uint64_t num_of_columns, struct column **columns);


struct column *get_column_by_name(char *name, uint64_t num_of_columns, struct column **columns);

