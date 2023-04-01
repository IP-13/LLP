#pragma once

#include <inttypes.h>
#include <bits/types/FILE.h>
#include <string.h>
#include <stdio.h>
#include "offsets.h"
#include "my_malloc.h"
#include "data_type.h"
#include "tuple.h"
#include "filter_type.h"
#include "db.h"

#define PAGE_SIZE 8192
#define NULL_PAGE UINT64_MAX
#define MAX_NUM_OF_TUPLES 4096


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
//

struct update_value {
    uint16_t attribute_num;
    void *value;
};

struct update_query {
    uint16_t num_of_updates;
    struct update_value **update_values;
};


void rewrite_page(FILE *file, file_offset old_offset, file_offset new_offset);

struct page *read_page_info(FILE *file, file_offset file_offset);

struct page *read_page(FILE *file, file_offset file_offset, enum data_type *table_scheme, uint64_t num_of_attributes);

int write_page(struct page *page, uint64_t num_of_attributes, const enum data_type *table_scheme, FILE *file);

struct page *create_page(file_offset offset, file_offset prev_page, file_offset next_page);

void free_page(struct page *page, const enum data_type *table_scheme, uint64_t num_of_attributes);

int is_empty_page(struct page *page);

void vacuum_page(struct page *page);

int insert_to_page(struct page *page, struct tuple *tuple);

struct page_select *select_from_page(struct page *page, uint16_t num_of_filters, struct filter **filters);

uint64_t delete_from_page(struct page *page, uint16_t num_of_filters, struct filter **filters,
                          enum data_type *table_scheme, uint64_t num_of_attributes);

void page_update(struct db *db, struct table *table, struct page *page, uint16_t num_of_filters,
                 struct filter **filters, struct update_query *update_query);

