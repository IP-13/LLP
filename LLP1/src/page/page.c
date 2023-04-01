#pragma once

#include "page.h"


uint16_t get_tuple_area() {
    return 3 * sizeof(file_offset) + sizeof(page_offset) + sizeof(uint64_t);
}

// size of free space in the end of the page
uint16_t get_free_space(struct page *page) {
    return PAGE_SIZE - page->free_space_offset.offset;
}


void rewrite_page(FILE *file, file_offset old_offset, file_offset new_offset) {
    struct page *page = read_page_info(file, old_offset);

    if (page->prev_page.offset != NULL_PAGE) {
        fseek(file, page->prev_page.offset + 2 * sizeof(file_offset), SEEK_SET);
        fwrite(&page->offset.offset, sizeof(file_offset), 1, file);
    }

    if (page->next_page.offset != NULL_PAGE) {
        fseek(file, page->next_page.offset + sizeof(file_offset), SEEK_SET);
        fwrite(&page->offset.offset, sizeof(file_offset), 1, file);
    }

    char *symbols = my_malloc(PAGE_SIZE * sizeof(char));

    fseek(file, old_offset.offset, SEEK_SET);
    fread(symbols, sizeof(char), PAGE_SIZE, file);

    fseek(file, new_offset.offset, SEEK_SET);
    fwrite(symbols, sizeof(char), PAGE_SIZE, file);

    my_free(symbols, PAGE_SIZE * sizeof(char));
    my_free(page, sizeof(struct page));
}


struct page *read_page_info(FILE *file, file_offset file_offset) {
    if (file_offset.offset == NULL_PAGE) {
        return NULL;
    }

    fseek(file, file_offset.offset, SEEK_SET);

    struct page *page = my_malloc(sizeof(struct page));

    fread(&page->offset.offset, sizeof(file_offset), 1, file);
    fread(&page->prev_page.offset, sizeof(file_offset), 1, file);
    fread(&page->next_page.offset, sizeof(file_offset), 1, file);
    fread(&page->free_space_offset.offset, sizeof(struct page_offset), 1, file);
    fread(&page->num_of_tuples, sizeof(uint64_t), 1, file);

    return page;
}


struct page *read_page(FILE *file, file_offset file_offset, enum data_type *table_scheme, uint64_t num_of_attributes) {
    if (file_offset.offset == NULL_PAGE) {
        return NULL;
    }

    struct page *page = read_page_info(file, file_offset);

    page->total_free_space_size = PAGE_SIZE - page->free_space_offset.offset;
    page->tuples = my_malloc(MAX_NUM_OF_TUPLES * sizeof(struct tuple *));

    for (size_t i = 0; i < page->num_of_tuples; i++) {
        page->tuples[i] = read_tuple(num_of_attributes, table_scheme, file);
    }

    return page;
}


int write_page(struct page *page, uint64_t num_of_attributes, const enum data_type *table_scheme, FILE *file) {
    vacuum_page(page);

    fseek(file, page->offset.offset, SEEK_SET);

    fwrite(&page->offset.offset, sizeof(file_offset), 1, file);
    fwrite(&page->prev_page.offset, sizeof(file_offset), 1, file);
    fwrite(&page->next_page.offset, sizeof(file_offset), 1, file);
    fwrite(&page->free_space_offset.offset, sizeof(struct page_offset), 1, file);
    fwrite(&page->num_of_tuples, sizeof(uint64_t), 1, file);

    for (size_t i = 0; i < page->num_of_tuples; i++) {
        int is_tuple_written = write_tuple(page->tuples[i], num_of_attributes, table_scheme, file);
        if (is_tuple_written == 0) {
            return 0;
        }
    }

    return 1;
}


struct page *create_page(file_offset offset, file_offset prev_page, file_offset next_page) {
    struct page *page = my_malloc(sizeof(struct page));

    page->offset.offset = offset.offset;
    page->prev_page.offset = prev_page.offset;
    page->next_page.offset = next_page.offset;
    page->free_space_offset.offset = get_tuple_area();
    page->num_of_tuples = 0;
    page->total_free_space_size = PAGE_SIZE - get_tuple_area();
    page->tuples = page->tuples = my_malloc(MAX_NUM_OF_TUPLES * sizeof(struct tuple *));

    return page;
}


void free_page(struct page *page, const enum data_type *table_scheme, uint64_t num_of_attributes) {
    for (size_t i = 0; i < page->num_of_tuples; i++) {
        free_tuple(page->tuples[i], table_scheme, num_of_attributes);
    }
    my_free(page->tuples, MAX_NUM_OF_TUPLES * sizeof(struct tuple *));
    my_free(page, sizeof(struct page));
}


int is_empty_page(struct page *page) {
    if (page->num_of_tuples == 0) {
        return 1;
    }

    return 0;
}


void vacuum_page(struct page *page) {
    if (page->num_of_tuples == 0) {
        return;
    }

    page->tuples[0]->start.offset = get_tuple_area();

    for (size_t i = 1; i < page->num_of_tuples; i++) {
        page->tuples[i]->start.offset = page->tuples[i - 1]->start.offset + page->tuples[i - 1]->size;
    }

    page->free_space_offset.offset =
            page->tuples[page->num_of_tuples - 1]->start.offset + page->tuples[page->num_of_tuples - 1]->size;

    page->total_free_space_size = PAGE_SIZE - page->free_space_offset.offset;
}


int insert_to_page(struct page *page, struct tuple *tuple) {
    if (page->total_free_space_size < tuple->size) {
        return 0;
    }

    if (get_free_space(page) < tuple->size) {
        vacuum_page(page);
    }

    tuple->start = page->free_space_offset;
    page->free_space_offset.offset += tuple->size;
    page->total_free_space_size -= tuple->size;
    page->tuples[page->num_of_tuples++] = tuple;

    return 1;
}


struct page_select *select_from_page(struct page *page, uint16_t num_of_filters, struct filter **filters) {
    struct page_select *page_select = my_malloc(sizeof(struct page_select));
    page_select->num_of_tuples = 0;
    page_select->tuples = my_malloc(MAX_NUM_OF_TUPLES * sizeof(struct tuple *));

    for (size_t i = 0; i < page->num_of_tuples; i++) {
        int is_failed = 0;

        for (size_t j = 0; j < num_of_filters; j++) {
            if (!is_match(page->tuples[i], filters[j])) {
                is_failed = 1;
                break;
            }
        }

        if (!is_failed) {
            page_select->tuples[page_select->num_of_tuples++] = page->tuples[i];
        }
    }

    return page_select;
}


uint64_t delete_from_page(struct page *page, uint16_t num_of_filters, struct filter **filters,
                          enum data_type *table_scheme, uint64_t num_of_attributes) {
    uint64_t deleted_tuples = 0;

    for (size_t i = 0; i < page->num_of_tuples; i++) {
        for (size_t j = 0; j < num_of_filters; j++) {
            if (!is_match(page->tuples[i], filters[j])) {
                break;
            }
            if (j == num_of_filters - 1) {
                struct tuple *temp = page->tuples[i];
                page->tuples[i] = page->tuples[page->num_of_tuples - 1];
                free_tuple(temp, table_scheme, num_of_attributes);
                deleted_tuples++;
                page->num_of_tuples--;
                i--;
            }
        }
    }

    if (deleted_tuples != 0) {
        vacuum_page(page);
    }

    return deleted_tuples;
}


void page_update(struct db *db, struct table *table, struct page *page, uint16_t num_of_filters,
                 struct filter **filters, struct update_query *update_query) {
    for (size_t i = 0; i < page->num_of_tuples; i++) {
        int is_failed = 0;

        for (size_t j = 0; j < num_of_filters; j++) {
            if (!is_match(page->tuples[i], filters[j])) {
                is_failed = 1;
                break;
            }
        }

        if (is_failed) {
            continue;
        }

        page->total_free_space_size += data_size(page->tuples[i]->data, table->table_scheme, table->num_of_columns);

        struct tuple *new_tuple = page->tuples[i];

        page->tuples[i] = page->tuples[page->num_of_tuples - 1];

        page->num_of_tuples--;

        for (size_t k = 0; k < update_query->num_of_updates; k++) {
            uint16_t update_attr_num = update_query->update_values[k]->attribute_num;
            void *new_value = update_query->update_values[k]->value;

            switch (table->table_scheme[update_attr_num]) {
                case BOOL: {
                    struct bool_field *bool_field = new_tuple->data[update_attr_num];
                    bool_field->data = *((int32_t *) new_value);
                    break;
                }
                case INT: {
                    struct int_field *int_field = new_tuple->data[update_attr_num];
                    int_field->data = *((int32_t *) new_value);
                    break;
                }
                case FLOAT: {
                    struct float_field *float_field = new_tuple->data[update_attr_num];
                    float_field->data = *((float *) new_value);
                    break;
                }
                case STRING: {
                    struct string_field *string_field = new_tuple->data[update_attr_num];
                    my_free(string_field->data, string_field->size * sizeof(char));
                    string_field->size = strlen((char *) new_value);
                    string_field->data = my_malloc(string_field->size * sizeof(char));
                    memcpy(string_field->data, new_value, string_field->size);
                    break;
                }
            }
        }

        new_tuple->size = data_size(new_tuple->data, table->table_scheme, table->num_of_columns);

        int is_inserted = insert_to_page(page, new_tuple);

        if (!is_inserted) {
            i--;

            int is_inserted_last_page = insert_to_page(table->last_page, new_tuple);

            if (!is_inserted_last_page) {
                struct page *new_page = create_page(db->last_page_offset, table->last_page->offset,
                                                    (file_offset) {.offset = NULL_PAGE});

                db->last_page_offset.offset += PAGE_SIZE;

                insert_to_page(new_page, new_tuple);

                table->last_page_offset = new_page->offset;
                table->last_page->next_page = new_page->offset;
                free_page(table->last_page, table->table_scheme, table->num_of_columns);
                table->last_page = new_page;
            }
        } else {
            struct tuple *temp = page->tuples[i];
            page->tuples[i] = page->tuples[page->num_of_tuples - 1];
            page->tuples[page->num_of_tuples - 1] = temp;
        }
    }
}

