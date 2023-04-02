#pragma once

#include <stdlib.h>

#include "my_malloc.h"
#include "db.h"
#include "column_list.h"
#include "table_list.h"
#include "table.h"
#include "filter_type.h"
#include "util.h"

// module tests
void test_column_list();

void test_table_list();

void open_db_test();

void tuple_test();

void page_test();

// stress tests
void insert_stress_test(int num_of_elements);

void select_stress_test(int num_of_elements, int num_of_selects);

// asymptotic tests

void delete_asymptotic_test(int num_of_elements);

void update_asymptotic_test(int num_of_elements);