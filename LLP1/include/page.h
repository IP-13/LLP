#pragma once

#include <bits/types/FILE.h>
#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "my_malloc.h"
#include "filters.h"
#include "tuple.h"


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