#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <bits/types/FILE.h>
#include "offsets.h"
#include "table.h"
#include "table_list.h"
#include "my_malloc.h"

#define DB_NAME_SIZE 40

struct db {
    char *name; // not in file
    FILE *file; // not in file
    uint64_t num_of_tables; // in file
    file_offset last_page_offset; // in file        space after last page
    struct table_list *table_list;  // in file
};

struct db *open_db(char *filename);

void close_db(struct db *db);

int add_table(struct db *db, struct table *table);

int delete_table(struct db *db, char *name);

void insert_to_table(struct db *db, char *table_name, struct tuple *tuple);

void print_tuple(struct tuple *tuple, uint32_t num_of_attributes, const enum data_type *table_scheme);

void select_from_table(struct db *db, char *table_name, uint16_t num_of_filters, struct filter **filters);

void delete_from_table(struct db *db, char *table_name, uint16_t num_of_filters, struct filter **filters);

void join_table(struct db *db, char *table1_name, char *table2_name, uint16_t num_of_filters1, struct filter **filters1,
                uint16_t num_of_filters2, struct filter **filters2);

void update_table(struct db *db, char *table_name, uint16_t num_of_filters, struct filter **filters,
                  struct update_query *update_query);

