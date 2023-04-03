#pragma once

#include <inttypes.h>
#include <bits/types/FILE.h>
#include "offsets.h"

#define PAGE_SIZE 8192
#define NULL_PAGE UINT64_MAX
#define MAX_NUM_OF_TUPLES 4096
#define MAX_STR_SIZE 1024
#define TABLE_NAME_SIZE 20
#define COLUMN_NAME_SIZE 20
#define DB_NAME_SIZE 40


enum data_type {
    BOOL = 0,
    INT = 1,
    FLOAT = 2,
    STRING = 3
};


struct tuple {
    page_offset start; // in file
    void **data; // in file
    uint16_t size; // not in file    data size
};


struct bool_field {
    int32_t data;
};


struct int_field {
    int32_t data;
};


struct float_field {
    float data;
};


struct string_field {
    uint16_t size;
    char *data;
};


struct page {
    file_offset offset; // in file
    file_offset prev_page; // in file
    file_offset next_page; // in file
    page_offset free_space_offset; // in file    offset to last free space chunk
    uint64_t num_of_tuples; // in file
    uint16_t total_free_space_size; // not in file
    struct tuple **tuples; // not in file
};


struct page_select {
    uint16_t num_of_tuples;
    struct tuple **tuples;
};


// TODO update_table
//enum update_type {
//    CONST,
//    INNER_REF,
//    OUTER_REF
//};


struct update_value {
    uint16_t attribute_num;
    void *value;
};


struct update_query {
    uint16_t num_of_updates;
    struct update_value **update_values;
};


struct column {
    enum data_type data_type; // in file
    uint64_t name_size; // in file
    char *name; // in file
};


struct column_list {
    struct column *value;
    struct column_list *next;
};


struct table {
    file_offset last_page_offset; // in file
    uint64_t name_size; // in file
    char *name; // in file
    uint64_t num_of_columns; // in file
    struct column_list *column_list;  // in file
    enum data_type *table_scheme; // not in file
    struct page *last_page; // not in file
};


struct table_list {
    struct table *value;
    struct table_list *next;
};


struct db {
    char *name; // not in file
    FILE *file; // not in file
    uint64_t num_of_tables; // in file
    file_offset last_page_offset; // in file        space after last page
    struct table_list *table_list;  // in file
};


enum filter_type {
    BOOL_EQ,
    BOOL_NEQ,
    INT_EQ,
    INT_NEQ,
    FLOAT_EQ,
    FLOAT_NEQ,
    STR_EQ,
    STR_NEQ,
    INT_GR,
    INT_LESS,
    INT_GR_EQ,
    INT_LESS_EQ,
    FLOAT_GR,
    FLOAT_LESS,
    FLOAT_GR_EQ,
    FLOAT_LESS_EQ,
    STR_LIKE
};


struct filter {
    uint16_t attribute_num; // number of attribute in tuple ot which filter_type is applied
    enum filter_type filter_type;
    void *value;
};


