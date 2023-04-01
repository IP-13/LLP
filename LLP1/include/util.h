#pragma once

#include <stdlib.h>
#include "my_malloc.h"
#include "db.h"
#include "tests.h"
#include "column_list.h"
#include "table_list.h"
#include "table.h"
#include "filter_type.h"

#define INT_RANGE 1000000
#define FLOAT_RANGE 1000
#define RANDOM_STRING_SIZE 20


// launch db
struct update_value *scanf_update_value(const enum data_type *table_scheme);

struct update_query *scanf_update_query(enum data_type *table_scheme);

void free_update_value(struct update_value *update_value, const enum data_type *table_scheme);

void free_update_query(struct update_query *update_query, enum data_type *table_scheme);

size_t str_len(const char *str);

char *scanf_table_name();

uint16_t scanf_num_of_filters();

void print_available_filters();

void *scanf_filter_value(enum filter_type filter_type);

struct filter **scanf_filters(uint32_t num_of_filters);

void free_filter_value(void *value, enum filter_type filter_type);

void free_filters(struct filter **filters, uint16_t num_of_filters);

struct tuple *scanf_tuple(struct table *table);

struct column_list *scanf_column_list(uint64_t num_of_columns);

struct table *scanf_table();

// tests util
struct table *create_test_table1();

struct table *create_test_table2();

struct table *create_test_table3();

void show_table(struct table *table);

void show_table_list(struct table_list *table_list);

void print_page(struct page *page, uint32_t num_of_attributes, enum data_type *table_scheme);

char *create_random_string(int32_t size);

float create_random_float();

struct tuple *create_random_tuple(uint64_t num_of_attributes, const enum data_type *table_scheme);

struct filter *create_random_filter(uint64_t num_of_attributes, const enum data_type *table_scheme);

struct filter **create_random_filters(uint16_t num_of_filters, uint64_t num_of_attrs, enum data_type *table_scheme);

void print_db_scheme(struct db *db);

