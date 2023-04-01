#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <bits/types/FILE.h>
#include "data_type.h"
#include "offsets.h"
#include "my_malloc.h"

#define MAX_STR_SIZE 1024


struct tuple {
    page_offset start; // in file
    void **data; // in file
    uint16_t size; // not in file    data size
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


struct tuple *read_tuple(uint64_t num_of_attributes, const enum data_type *table_scheme, FILE *file);

int write_tuple(struct tuple *tuple, uint64_t num_of_attributes, const enum data_type *table_scheme, FILE *file);

struct tuple *create_tuple(page_offset start, void **data,
                           uint64_t num_of_attributes, const enum data_type *table_scheme);

void free_tuple(struct tuple *tuple, const enum data_type *table_scheme, uint64_t num_of_attributes);

uint16_t data_size(void **data, const enum data_type *table_scheme, uint64_t num_of_attributes);

