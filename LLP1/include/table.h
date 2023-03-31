#pragma once

#include <inttypes.h>
#include <bits/types/FILE.h>
#include <stdio.h>
#include "offsets.h"
#include "page.h"
#include "data_type.h"
#include "column_list.h"

#define TABLE_NAME_SIZE 20
#define COLUMN_NAME_SIZE 20

struct table {
    file_offset last_page_offset; // in file
    uint64_t name_size; // in file
    char *name; // in file
    uint64_t num_of_columns; // in file
    struct column_list *column_list;  // in file
    enum data_type *table_scheme; // not in file
    struct page *last_page; // not in file
};

struct column {
    enum data_type data_type; // in file
    uint64_t name_size; // in file
    char *name; // in file
};

struct table *create_table(uint64_t name_size, char *name, uint64_t num_of_columns, struct column_list *column_list);

void free_table(struct table *table);

struct column *create_column(enum data_type data_type, uint64_t name_size, char *name);

void free_column(struct column *column);

