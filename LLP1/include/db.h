#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <bits/types/FILE.h>
#include <string.h>
#include "util.h"
#include "page.h"
#include "my_malloc.h"
#include "offsets.h"
#include "table_list.h"
#include "column_list.h"
#include "filters.h"
#include "structures.h"


struct db *open_db(char *filename);

void close_db(struct db *db);

int add_table(struct db *db, struct table *table);

int delete_table(struct db *db, char *name);

void insert_to_table(struct db *db, char *table_name, struct tuple *tuple);

void select_from_table(struct db *db, char *table_name, uint16_t num_of_filters, struct filter **filters);

void delete_from_table(struct db *db, char *table_name, uint16_t num_of_filters, struct filter **filters);

void join_table(struct db *db, char *table1_name, char *table2_name, uint16_t num_of_filters1, struct filter **filters1,
                uint16_t num_of_filters2, struct filter **filters2);

void update_table(struct db *db, char *table_name, uint16_t num_of_filters, struct filter **filters,
                  struct update_query *update_query);

