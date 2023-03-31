#pragma once

#include <stdlib.h>

#include "my_malloc.h"
#include "db.h"
#include "column_list.h"
#include "table_list.h"
#include "table.h"
#include "filter_type.h"

// util
struct table *create_test_table1();

struct table *create_test_table2();

struct table *create_test_table3();

void show_table(struct table *table);

void show_table_list(struct table_list *table_list);

void print_page(struct page *page, uint32_t num_of_attributes, enum data_type *table_scheme);

char *create_random_string(int32_t size);

struct tuple *create_random_tuple(uint64_t num_of_attributes, const enum data_type *table_scheme);

void print_db_scheme(struct db *db);


// module tests
void test_column_list();

void test_table_list();

void open_db_test();

void tuple_test();

void page_test();


// stress tests
void insert_test();

