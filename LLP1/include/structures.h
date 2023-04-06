#pragma once

#include <inttypes.h>
#include <bits/types/FILE.h>
#include "offsets.h"

#define MAX_STR_SIZE 1024
#define TABLE_NAME_SIZE 20
#define COLUMN_NAME_SIZE 20
#define DB_NAME_SIZE 40


struct update_value {
    uint16_t attribute_num;
    void *value;
};


struct update_query {
    uint16_t num_of_updates;
    struct update_value **update_values;
};

