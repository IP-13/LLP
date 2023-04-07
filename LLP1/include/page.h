#pragma once

#include <bits/types/FILE.h>
#include <stdlib.h>
#include <stdio.h>
#include "my_malloc.h"
#include "filters.h"
#include "tuple.h"

#define PAGE_SIZE 8192
#define NULL_PAGE UINT64_MAX
#define MAX_NUM_OF_TUPLES 4096


struct page {
    file_offset offset; // in file
    file_offset prev_page; // in file
    file_offset next_page; // in file
    uint64_t num_of_tuples; // in file
    uint16_t total_free_space_size; // not in file
    struct tuple **tuples; // not in file
};


struct page_select {
    uint16_t num_of_tuples;
    struct tuple **tuples;
};


void rewrite_page(file_offset old_offset, file_offset new_offset, FILE *file);


struct page *read_page_info(file_offset file_offset, FILE *file);


struct page *read_page(file_offset file_offset, uint64_t num_of_columns, enum data_type *table_scheme, FILE *file);


int write_page(struct page *page, uint64_t num_of_columns, const enum data_type *table_scheme, FILE *file);


struct page *create_empty_page(file_offset offset, file_offset prev_page, file_offset next_page);


void free_page(struct page *page, uint64_t num_of_columns, const enum data_type *table_scheme);


int insert_to_page(struct page *page, struct tuple *tuple);


struct page_select *select_from_page(struct page *page, uint64_t num_of_columns, struct column **columns,
                                     uint16_t num_of_filters, struct filter **filters);


uint16_t delete_from_page(struct page *page, uint64_t num_of_columns, struct column **columns,
                          enum data_type *table_scheme, uint16_t num_of_filters, struct filter **filters);

