#pragma once

#include "my_malloc.h"
#include "column_list.h"
#include "structures.h"


struct table *create_table(uint64_t name_size, char *name, uint64_t num_of_columns, struct column_list *column_list);

void free_table(struct table *table);

struct column *create_column(enum data_type data_type, uint64_t name_size, char *name);

void free_column(struct column *column);

