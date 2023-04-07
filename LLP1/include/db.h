#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <bits/types/FILE.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#include "table.h"
#include "page.h"
#include "my_malloc.h"
#include "offsets.h"
#include "filters.h"
#include "tuple.h"

#define TABLES_INITIAL_CAPACITY 10


struct db {
    char *name; // not in file
    FILE *file; // not in file
    file_offset last_page_offset; // in file        space after last page
    uint64_t num_of_tables; // in file
    uint64_t tables_capacity;
    struct table **tables;  // in file
};


struct join_values {
    uint16_t num_of_join_values;
    char **table1_column_names;
    char **table2_column_names;
    enum filter_cond *filter_cond;
};


struct update_value {
    uint16_t attribute_num;
    void *value;
};


struct update_query {
    uint16_t num_of_updates;
    struct update_value **update_values;
};


struct db *open_db(char *filename);


void close_db(struct db *db);


int add_table(struct db *db, struct table *table);


int delete_table(struct db *db, char *table_name);


void insert_to_table(struct db *db, char *table_name, struct tuple *tuple);


void select_from_table(struct db *db, char *table_name, uint16_t num_of_filters, struct filter **filters);


void delete_from_table(struct db *db, char *table_name, uint16_t num_of_filters, struct filter **filters);


void join_table(struct db *db, char *table1_name, char *table2_name, uint16_t num_of_filters1, struct filter **filters1,
                uint16_t num_of_filters2, struct filter **filters2, struct join_values *join_values);


void update_table(struct db *db, char *table_name, uint16_t num_of_filters, struct filter **filters,
                  struct update_query *update_query);

