#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "my_malloc.h"
#include "structures.h"
#include "page.h"
#include "column.h"


struct table {
    file_offset last_page_offset; // in file
    uint64_t name_size; // in file
    char *name; // in file
    uint64_t num_of_columns; // in file
    struct column **columns;  // in file
    enum data_type *table_scheme; // not in file
    struct page *last_page; // not in file
};


struct table *create_table(file_offset last_page_offset, uint64_t name_size, char *name,
                           uint64_t num_of_columns, struct column **columns);


void free_table(struct table *table);


struct table *get_table_by_name(char *name, uint64_t num_of_tables, struct table **tables);

