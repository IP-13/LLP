#pragma once

#include "page.h"


static uint16_t page_static_info_size() {
    return 3 * sizeof(file_offset) + sizeof(uint64_t);
}


void rewrite_page(file_offset old_offset, file_offset new_offset, FILE *file) {
    if (old_offset.offset == new_offset.offset) {
        return;
    }

    struct page *page = read_page_info(old_offset, file);

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


struct page *read_page_info(file_offset file_offset, FILE *file) {
    if (file_offset.offset == NULL_PAGE) {
        return NULL;
    }

    fseek(file, file_offset.offset, SEEK_SET);

    struct page *page = my_malloc(sizeof(struct page));

    fread(&page->offset.offset, sizeof(file_offset), 1, file);
    fread(&page->prev_page.offset, sizeof(file_offset), 1, file);
    fread(&page->next_page.offset, sizeof(file_offset), 1, file);
    fread(&page->num_of_tuples, sizeof(uint64_t), 1, file);

    return page;
}


struct page *read_page(file_offset file_offset, uint64_t num_of_columns, enum data_type *table_scheme, FILE *file) {
    if (file_offset.offset == NULL_PAGE) {
        return NULL;
    }

    struct page *page = read_page_info(file_offset, file);

    page->total_free_space_size = PAGE_SIZE - page_static_info_size();
    page->tuples = my_malloc(MAX_NUM_OF_TUPLES * sizeof(struct tuple *));

    for (size_t i = 0; i < page->num_of_tuples; i++) {
        page->tuples[i] = read_tuple(num_of_columns, table_scheme, file);
        page->total_free_space_size -= page->tuples[i]->data_size;
    }

    return page;
}


int write_page(struct page *page, uint64_t num_of_columns, const enum data_type *table_scheme, FILE *file) {
    fseek(file, page->offset.offset, SEEK_SET);

    fwrite(&page->offset.offset, sizeof(file_offset), 1, file);
    fwrite(&page->prev_page.offset, sizeof(file_offset), 1, file);
    fwrite(&page->next_page.offset, sizeof(file_offset), 1, file);
    fwrite(&page->num_of_tuples, sizeof(uint64_t), 1, file);

    for (size_t i = 0; i < page->num_of_tuples; i++) {
        write_tuple(page->tuples[i], num_of_columns, table_scheme, file);
    }

    return 1;
}


struct page *create_empty_page(file_offset offset, file_offset prev_page, file_offset next_page) {
    struct page *page = my_malloc(sizeof(struct page));

    page->offset.offset = offset.offset;
    page->prev_page.offset = prev_page.offset;
    page->next_page.offset = next_page.offset;
    page->num_of_tuples = 0;
    page->total_free_space_size = PAGE_SIZE - page_static_info_size();
    page->tuples = page->tuples = my_malloc(MAX_NUM_OF_TUPLES * sizeof(struct tuple *));

    return page;
}


void free_page(struct page *page, uint64_t num_of_columns, const enum data_type *table_scheme) {
    for (size_t i = 0; i < page->num_of_tuples; i++) {
        free_tuple(page->tuples[i], num_of_columns, table_scheme);
    }

    my_free(page->tuples, MAX_NUM_OF_TUPLES * sizeof(struct tuple *));
    my_free(page, sizeof(struct page));
}


int insert_to_page(struct page *page, struct tuple *tuple) {
    if (page->total_free_space_size < tuple->data_size) {
        return 0;
    }

    page->total_free_space_size -= tuple->data_size;
    page->tuples[page->num_of_tuples++] = tuple;

    return 1;
}


struct page_select *select_from_page(struct page *page, uint64_t num_of_columns, struct column **columns,
                                     uint16_t num_of_filters, struct filter **filters) {
    struct page_select *page_select = my_malloc(sizeof(struct page_select));
    page_select->num_of_tuples = 0;
    page_select->tuples = my_malloc(MAX_NUM_OF_TUPLES * sizeof(struct tuple *));

    for (size_t i = 0; i < page->num_of_tuples; i++) {
        int is_failed = 0;

        for (size_t j = 0; j < num_of_filters; j++) {
            if (!is_match(page->tuples[i], filters[j], num_of_columns, columns)) {
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


uint16_t delete_from_page(struct page *page, uint64_t num_of_columns, struct column **columns,
                          enum data_type *table_scheme, uint16_t num_of_filters, struct filter **filters) {
    uint64_t deleted_tuples = 0;

    for (size_t i = 0; i < page->num_of_tuples; i++) {
        int is_failed = 0;

        for (size_t j = 0; j < num_of_filters; j++) {
            if (!is_match(page->tuples[i], filters[j], num_of_columns, columns)) {
                is_failed = 1;
                break;
            }
        }

        if (!is_failed) {
            struct tuple *temp = page->tuples[i];
            page->tuples[i] = page->tuples[page->num_of_tuples - 1];
            page->total_free_space_size += data_size(temp->data, num_of_columns, table_scheme);
            free_tuple(temp, num_of_columns, table_scheme);
            deleted_tuples++;
            page->num_of_tuples--;
            i--;
        }
    }

    return deleted_tuples;
}


//struct page *page_update(struct page *page, uint16_t num_of_filters, struct filter **filters,
//                         struct update_query *update_query,
//) {
//    for (size_t i = 0; i < page->num_of_tuples; i++) {
//        int is_failed = 0;
//
//        for (size_t j = 0; j < num_of_filters; j++) {
//            if (!is_match(page->tuples[i], filters[j])) {
//                is_failed = 1;
//                break;
//            }
//        }
//
//        if (is_failed) {
//            continue;
//        }
//
//        page->total_free_space_size +=
//                (data_size(page->tuples[i]->data, table->table_scheme, table->num_of_columns) + sizeof(uint16_t));
//
//        struct tuple *new_tuple = page->tuples[i];
//
//        page->tuples[i] = page->tuples[page->num_of_tuples - 1];
//
//        page->num_of_tuples--;
//
//        for (size_t k = 0; k < update_query->num_of_updates; k++) {
//            uint16_t update_attr_num = update_query->update_values[k]->attribute_num;
//            void *new_value = update_query->update_values[k]->value;
//
//            switch (table->table_scheme[update_attr_num]) {
//                case BOOL: {
//                    struct bool_field *bool_field = new_tuple->data[update_attr_num];
//                    bool_field->data = *((int32_t *) new_value);
//                    break;
//                }
//                case INT: {
//                    struct int_field *int_field = new_tuple->data[update_attr_num];
//                    int_field->data = *((int32_t *) new_value);
//                    break;
//                }
//                case FLOAT: {
//                    struct float_field *float_field = new_tuple->data[update_attr_num];
//                    float_field->data = *((float *) new_value);
//                    break;
//                }
//                case STRING: {
//                    struct string_field *string_field = new_tuple->data[update_attr_num];
//                    my_free(string_field->data, string_field->size * sizeof(char));
//                    string_field->size = strlen((char *) new_value);
//                    string_field->data = my_malloc(string_field->size * sizeof(char));
//                    memcpy(string_field->data, new_value, string_field->size);
//                    break;
//                }
//            }
//        }
//
//        new_tuple->size = data_size(new_tuple->data, table->table_scheme, table->num_of_columns);
//
//        int is_inserted = insert_to_page(page, new_tuple);
//
//        if (!is_inserted) {
//            i--;
//
//            int is_inserted_last_page = insert_to_page(table->last_page, new_tuple);
//
//            if (!is_inserted_last_page) {
//                struct page *new_page = create_empty_page(db->last_page_offset, table->last_page->offset,
//                                                          (file_offset) {.offset = NULL_PAGE});
//
//                db->last_page_offset.offset += PAGE_SIZE;
//
//                insert_to_page(new_page, new_tuple);
//
//                table->last_page_offset = new_page->offset;
//                table->last_page->next_page = new_page->offset;
//                free_page(table->last_page, table->num_of_columns, table->table_scheme);
//                table->last_page = new_page;
//            }
//        } else {
//            struct tuple *temp = page->tuples[i];
//            page->tuples[i] = page->tuples[page->num_of_tuples - 1];
//            page->tuples[page->num_of_tuples - 1] = temp;
//        }
//    }
//}
//
