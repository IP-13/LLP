#pragma once

#include <bits/types/FILE.h>
#include <stdio.h>
#include "my_malloc.h"
#include "offsets.h"
#include "data_type.h"


struct tuple {
    uint16_t data_size; // not in file
    void **data; // in file
};


struct bool_field {
    int32_t data;
};


struct int_field {
    int32_t data;
};


struct float_field {
    float data;
};


struct string_field {
    uint16_t size;
    char *data;
};


struct tuple *read_tuple(uint64_t num_of_columns, const enum data_type *table_scheme, FILE *file);


int write_tuple(struct tuple *tuple, uint64_t num_of_columns, const enum data_type *table_scheme, FILE *file);


struct tuple *create_tuple(void **data, uint64_t num_of_columns, const enum data_type *table_scheme);


void free_tuple(struct tuple *tuple, uint64_t num_of_columns, const enum data_type *table_scheme);


uint16_t data_size(void **data, uint64_t num_of_columns, const enum data_type *table_scheme);

