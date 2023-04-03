#pragma once

#include <bits/types/FILE.h>
#include <stdio.h>
#include "my_malloc.h"
#include "structures.h"


struct tuple *read_tuple(uint64_t num_of_attributes, const enum data_type *table_scheme, FILE *file);

int write_tuple(struct tuple *tuple, uint64_t num_of_attributes, const enum data_type *table_scheme, FILE *file);

struct tuple *create_tuple(page_offset start, void **data,
                           uint64_t num_of_attributes, const enum data_type *table_scheme);

void free_tuple(struct tuple *tuple, const enum data_type *table_scheme, uint64_t num_of_attributes);

uint16_t data_size(void **data, const enum data_type *table_scheme, uint64_t num_of_attributes);