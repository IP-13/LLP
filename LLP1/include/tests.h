#pragma once

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "my_malloc.h"
#include "db.h"
#include "table.h"
#include "filters.h"
#include "tuple.h"
#include "util.h"

// module tests
void open_db_test();

void tuple_test();

void page_test();

void insert_from_file(char *db_filename, char *table_name, char *data_file_name);

// stress tests
void insert_stress_test(int num_of_elements);

void select_stress_test(int num_of_elements, int num_of_selects);

// asymptotic tests

void delete_asymptotic_test(int max_num_of_elements);

void update_asymptotic_test(int max_num_of_elements);