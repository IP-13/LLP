#pragma once

#include "db.h"

static struct db *create_db(char *filename) {
    struct db *db = my_malloc(sizeof(struct db));

    db->name = filename;
    db->file = fopen(filename, "w+");;
    db->last_page_offset = (file_offset) {.offset = 0};
    db->num_of_tables = 0;
    db->tables_capacity = TABLES_INITIAL_CAPACITY;
    db->tables = my_malloc(TABLES_INITIAL_CAPACITY * sizeof(struct table *));

    return db;
}


static struct column **read_columns(uint64_t num_of_columns, FILE *file) {
    struct column **columns = my_malloc(num_of_columns * sizeof(struct column *));

    for (size_t i = 0; i < num_of_columns; i++) {
        enum data_type data_type;
        fread(&data_type, sizeof(enum data_type), 1, file);

        uint64_t name_size;
        fread(&name_size, sizeof(uint64_t), 1, file);

        char *name = my_malloc(name_size * sizeof(char));
        fread(name, sizeof(char), name_size, file);

        columns[i] = create_column(data_type, name_size, name);
    }

    return columns;
}


static struct table **read_tables(uint64_t num_of_tables, file_offset db_last_page_offset, FILE *file) {
    struct table **tables = my_malloc(2 * num_of_tables * sizeof(struct table *));

    fseek(file, db_last_page_offset.offset, SEEK_SET);

    for (size_t i = 0; i < num_of_tables; i++) {
        file_offset last_page_offset;
        fread(&last_page_offset.offset, sizeof(uint64_t), 1, file);

        uint64_t name_size;
        fread(&name_size, sizeof(uint64_t), 1, file);

        char *name = my_malloc(name_size * sizeof(char));
        fread(name, sizeof(char), name_size, file);

        uint64_t num_of_columns;
        fread(&num_of_columns, sizeof(uint64_t), 1, file);

        struct column **columns = read_columns(num_of_columns, file);

        enum data_type *table_scheme = my_malloc(num_of_columns * sizeof(enum data_type));

        for (size_t j = 0; j < num_of_columns; j++) {
            table_scheme[j] = columns[i]->data_type;
        }

        struct table *table = create_table(last_page_offset, name_size, name, num_of_columns, columns);

        tables[i] = table;
    }

    return tables;
}


static void free_columns(uint64_t num_of_columns, struct column **columns) {
    for (size_t i = 0; i < num_of_columns; i++) {
        struct column *column = columns[i];
        my_free(column->name, column->name_size * sizeof(char));
        my_free(column, sizeof(struct column));
    }

    my_free(columns, num_of_columns * sizeof(struct column *));
}


static void free_tables(uint64_t num_of_tables, struct table **tables) {
    for (size_t i = 0; i < num_of_tables; i++) {
        struct table *table = tables[i];
        free_page(table->last_page, table->num_of_columns, table->table_scheme);
        my_free(table->table_scheme, table->num_of_columns * sizeof(enum data_type));
        free_columns(table->num_of_columns, table->columns);
        my_free(table->name, table->name_size * sizeof(char));
        my_free(table, sizeof(struct table));
    }

    my_free(tables, num_of_tables * sizeof(struct table *));
}


static void write_columns(size_t num_of_columns, struct column **columns, FILE *file) {
    for (size_t i = 0; i < num_of_columns; i++) {
        struct column *column = columns[i];
        fwrite(&column->data_type, sizeof(enum data_type), 1, file);
        fwrite(&column->name_size, sizeof(uint64_t), 1, file);
        fwrite(column->name, sizeof(char), column->name_size, file);
    }
}


static void write_tables(size_t num_of_tables, struct table **tables, FILE *file) {
    for (size_t i = 0; i < num_of_tables; i++) {
        struct table *table = tables[i];
        fwrite(&table->last_page_offset.offset, sizeof(table->last_page_offset.offset), 1, file);
        fwrite(&table->name_size, sizeof(table->name_size), 1, file);
        fwrite(table->name, sizeof(char), table->name_size, file);
        fwrite(&table->num_of_columns, sizeof(table->num_of_columns), 1, file);
        write_columns(table->num_of_columns, table->columns, file);
    }

    for (size_t i = 0; i < num_of_tables; i++) {
        struct table *table = tables[i];
        write_page(table->last_page, table->num_of_columns, table->table_scheme, file);
    }
}

// checks if last db page is last page of any table
static void check_if_last_page(file_offset last_db_page_offset, file_offset new_offset, struct db *db) {
    for (size_t i = 0; i < db->num_of_tables; i++) {
        struct table *curr_table = db->tables[i];
        if (curr_table->last_page_offset.offset == last_db_page_offset.offset) {
            curr_table->last_page_offset.offset = new_offset.offset;
            curr_table->last_page->offset.offset = new_offset.offset;
        }
    }
}


static int32_t relocate_tuples(struct page *source, struct page *dest) {
    if (source->offset.offset == dest->offset.offset) {
        return 1;
    }

    while (source->num_of_tuples > 0) {
        struct tuple *last_tuple = source->tuples[source->num_of_tuples - 1];

        int is_inserted = insert_to_page(dest, last_tuple);

        if (!is_inserted) {
            return 1;
        }

        source->num_of_tuples--;
        source->total_free_space_size += last_tuple->data_size;
    }

    return 0;
}


static void handle_page_select(struct page_select *page_select, enum data_type *table_scheme,
                               uint64_t num_of_columns) {
#ifndef TEST
    for (size_t i = 0; i < page_select->num_of_tuples; i++) {
        print_tuple(page_select->tuples[i], num_of_columns, table_scheme);
        printf("\n");
    }
#endif
    my_free(page_select->tuples, MAX_NUM_OF_TUPLES * sizeof(struct tuple *));
    my_free(page_select, sizeof(struct page_select));
}


struct db *open_db(char *filename) {
    FILE *file = fopen(filename, "r+");

    if (file == NULL) {
        return create_db(filename);
    }

    struct db *db = my_malloc(sizeof(struct db));

    file_offset last_page_offset;
    fseek(file, -8, SEEK_END);
    fread(&last_page_offset.offset, sizeof(uint64_t), 1, file);

    uint64_t num_of_tables;
    fseek(file, -16, SEEK_END);
    fread(&num_of_tables, sizeof(last_page_offset.offset), 1, file);

    uint64_t tables_capacity;
    fseek(file, -24, SEEK_END);
    fread(&tables_capacity, sizeof(last_page_offset.offset), 1, file);

    db->name = filename;
    db->file = file;
    db->last_page_offset = last_page_offset;
    db->num_of_tables = num_of_tables;
    db->tables_capacity = tables_capacity;
    db->tables = read_tables(num_of_tables, last_page_offset, file);

    for (size_t i = 0; i < db->num_of_tables; i++) {
        struct table *table = db->tables[i];
        struct page *page = read_page(table->last_page_offset, table->num_of_columns, table->table_scheme, file);
        table->last_page = page;
    }

    return db;
}


void close_db(struct db *db) {
    fseek(db->file, 0, SEEK_SET);
    truncate(db->name, db->last_page_offset.offset);
    fseek(db->file, db->last_page_offset.offset, SEEK_SET);
    write_tables(db->num_of_tables, db->tables, db->file);
    fseek(db->file, 0, SEEK_END);
    fwrite(&(db->tables_capacity), sizeof(db->tables_capacity), 1, db->file);
    fwrite(&(db->num_of_tables), sizeof(db->num_of_tables), 1, db->file);
    fwrite(&(db->last_page_offset.offset), sizeof(db->last_page_offset.offset), 1, db->file);
    free_tables(db->num_of_tables, db->tables);
    fclose(db->file);
    my_free(db->name, strlen(db->name) * sizeof(char));
    my_free(db, sizeof(struct db));
}


static struct table **copy_tables(uint64_t num_of_tables, struct table **tables, uint64_t new_capacity) {
    struct table **tables_copy = my_malloc(new_capacity * sizeof(struct table *));

    for (size_t i = 0; i < num_of_tables; i++) {
        tables_copy[i] = tables[i];
    }

    my_free(tables, num_of_tables * sizeof(struct table *));

    return tables_copy;
}


int add_table(struct db *db, struct table *table) {
    if (db->tables == NULL) {
        db->tables = my_malloc(TABLES_INITIAL_CAPACITY * sizeof(struct table *));
        db->tables_capacity = TABLES_INITIAL_CAPACITY;
    }

    if (get_table_by_name(table->name, db->num_of_tables, db->tables) != NULL) {
        return 0;
    }

    if (db->num_of_tables == db->tables_capacity) {
        copy_tables(db->num_of_tables, db->tables, db->num_of_tables * 2);
        db->tables_capacity *= 2;
    }

    table->last_page_offset.offset = db->last_page_offset.offset;

    table->last_page = create_empty_page(table->last_page_offset,
                                         (file_offset) {.offset = NULL_PAGE},
                                         (file_offset) {.offset=NULL_PAGE});

    db->last_page_offset.offset += PAGE_SIZE;

    write_page(table->last_page, table->num_of_columns, table->table_scheme, db->file);

    db->tables[db->num_of_tables++] = table;

    return 1;
}


int delete_table(struct db *db, char *table_name) {
    struct table *table = get_table_by_name(table_name, db->num_of_tables, db->tables);

    if (table == NULL) {
        return -1;
    }

    file_offset delete_page_offset = table->last_page_offset;

    while (delete_page_offset.offset != NULL_PAGE) {
        struct page *curr_page_info = read_page_info(delete_page_offset, db->file);
        db->last_page_offset.offset -= PAGE_SIZE;
        file_offset last_page_offset = db->last_page_offset;

        if (delete_page_offset.offset != last_page_offset.offset) {
            rewrite_page(last_page_offset, delete_page_offset, db->file);
            check_if_last_page(last_page_offset, delete_page_offset, db);
        }

        delete_page_offset.offset = curr_page_info->prev_page.offset;

        my_free(curr_page_info, sizeof(struct page));
    }


    for (size_t i = 0; i < db->num_of_tables; i++) {
        if (db->tables[i] == table) {
            db->tables[i] = db->tables[db->num_of_tables - 1];
        }
    }

    db->num_of_tables--;

    free_table(table);

    return 1;
}


void insert_to_table(struct db *db, char *table_name, struct tuple *tuple) {
    struct table *table = get_table_by_name(table_name, db->num_of_tables, db->tables);

    if (table == NULL) {
        return;
    }

    int is_inserted = insert_to_page(table->last_page, tuple);

    if (!is_inserted) {
        file_offset offset = db->last_page_offset;
        file_offset prev_page = table->last_page_offset;
        file_offset next_page = (file_offset) {.offset = NULL_PAGE};

        struct page *new_last_page = create_empty_page(offset, prev_page, next_page);

        table->last_page->next_page = new_last_page->offset;

        write_page(table->last_page, table->num_of_columns, table->table_scheme, db->file);

        free_page(table->last_page, table->num_of_columns, table->table_scheme);

        table->last_page_offset = new_last_page->offset;
        table->last_page = new_last_page;

        db->last_page_offset.offset += PAGE_SIZE;

        insert_to_page(table->last_page, tuple);
    }
}


void select_from_table(struct db *db, char *table_name, uint16_t num_of_filter, struct filter **filters) {
    struct table *table = get_table_by_name(table_name, db->num_of_tables, db->tables);

    if (table == NULL) {
        return;
    }

    struct page_select *last_page_page_select = select_from_page(table->last_page, table->num_of_columns,
                                                                 table->columns, num_of_filter, filters);

    handle_page_select(last_page_page_select, table->table_scheme, table->num_of_columns);


    struct page *curr_page = read_page(table->last_page->prev_page, table->num_of_columns, table->table_scheme,
                                       db->file);

    while (curr_page != NULL) {
        struct page_select *page_select = select_from_page(curr_page, table->num_of_columns, table->columns,
                                                           num_of_filter, filters);

        handle_page_select(page_select, table->table_scheme, table->num_of_columns);

        file_offset prev_page = curr_page->prev_page;

        free_page(curr_page, table->num_of_columns, table->table_scheme);

        curr_page = read_page(prev_page, table->num_of_columns, table->table_scheme, db->file);
    }
}


void delete_from_table(struct db *db, char *table_name, uint16_t num_of_filters, struct filter **filters) {
    struct table *table = get_table_by_name(table_name, db->num_of_tables, db->tables);

    if (table == NULL) {
        return;
    }

    delete_from_page(table->last_page, table->num_of_columns, table->columns,
                     table->table_scheme, num_of_filters, filters);

    struct page *curr_page = read_page(table->last_page->prev_page, table->num_of_columns,
                                       table->table_scheme, db->file);

    while (curr_page != NULL) {
        delete_from_page(curr_page, table->num_of_columns, table->columns,
                         table->table_scheme, num_of_filters, filters);

        int32_t relocate_res = relocate_tuples(table->last_page, curr_page);

        if (relocate_res == 0) {
            db->last_page_offset.offset -= PAGE_SIZE;
            check_if_last_page(db->last_page_offset, table->last_page_offset, db);
            rewrite_page(table->last_page_offset, db->last_page_offset, db->file);

            file_offset prev_page = table->last_page->prev_page;

            free_page(table->last_page, table->num_of_columns, table->table_scheme);

            table->last_page_offset = prev_page;
            table->last_page = read_page(prev_page, table->num_of_columns, table->table_scheme, db->file);


            relocate_tuples(table->last_page, curr_page);
        }

        file_offset prev_page = curr_page->prev_page;

        write_page(curr_page, table->num_of_columns, table->table_scheme, db->file);
        free_page(curr_page, table->num_of_columns, table->table_scheme);
        curr_page = read_page(prev_page, table->num_of_columns, table->table_scheme, db->file);
    }
}


static int32_t check_join_values(struct join_values *join_values, struct tuple *tuple1, struct tuple *tuple2,
                                 uint64_t num_of_columns1, struct column **columns1, uint64_t num_of_columns2,
                                 struct column **columns2) {
    int32_t num_of_success = 0;

    for (size_t i = 0; i < join_values->num_of_join_values; i++) {
        struct filter *filter = my_malloc(sizeof(struct filter));
        filter->col_name = join_values->table1_column_names[i];
        filter->filter_type = CONST;
        filter->filter_cond = join_values->filter_cond[i];
        filter->value = tuple2->data[get_column_index(join_values->table2_column_names[i], num_of_columns2, columns2)];
        if (is_match(tuple1, filter, num_of_columns1, columns1)) {
            num_of_success++;
        }
        my_free(filter, sizeof(struct filter));
    }

    if (num_of_success == join_values->num_of_join_values) {
        return 1;
    }

    return 0;
}


void join_table(struct db *db, char *table1_name, char *table2_name, uint16_t num_of_filters1, struct filter **filters1,
                uint16_t num_of_filters2, struct filter **filters2, struct join_values *join_values) {
    struct table *table1 = get_table_by_name(table1_name, db->num_of_tables, db->tables);

    if (table1 == NULL) {
        return;
    }

    struct table *table2 = get_table_by_name(table2_name, db->num_of_tables, db->tables);

    if (table2 == NULL) {
        return;
    }

    struct page *curr_page1 = table1->last_page;

    while (curr_page1 != NULL) {
        struct page_select *page_select1 = select_from_page(curr_page1, table1->num_of_columns, table1->columns,
                                                            num_of_filters1, filters1);

        struct page *curr_page2 = table2->last_page;

        while (curr_page2 != NULL) {
            struct page_select *page_select2 = select_from_page(curr_page2, table2->num_of_columns, table2->columns,
                                                                num_of_filters2, filters2);

            for (size_t i = 0; i < page_select1->num_of_tuples; i++) {
                for (size_t j = 0; j < page_select2->num_of_tuples; j++) {
                    struct tuple *tuple1 = page_select1->tuples[i];
                    struct tuple *tuple2 = page_select2->tuples[j];

                    if (check_join_values(join_values, tuple1, tuple2, table1->num_of_columns,
                                          table1->columns, table2->num_of_columns, table2->columns)) {
                        print_tuple(tuple1, table1->num_of_columns, table1->table_scheme);
                        print_tuple(tuple2, table2->num_of_columns, table2->table_scheme);
                        printf("\n");
                    }
                }
            }

            file_offset prev_page = curr_page2->prev_page;

            if (curr_page2 != table2->last_page) {
                free_page(curr_page2, table2->num_of_columns, table2->table_scheme);
            }

            curr_page2 = read_page(prev_page, table2->num_of_columns, table2->table_scheme, db->file);
        }

        file_offset prev_page = curr_page1->prev_page;

        if (curr_page1 != table1->last_page) {
            free_page(curr_page1, table1->num_of_columns, table1->table_scheme);
        }

        curr_page1 = read_page(prev_page, table1->num_of_columns, table1->table_scheme, db->file);
    }
}


static void update_tuple(struct tuple *tuple, struct update_query *update_query, struct table *table) {
    for (size_t i = 0; i < update_query->num_of_updates; i++) {
        uint16_t update_attr_num = update_query->update_values[i]->attribute_num;
        void *new_value = update_query->update_values[i]->value;

        switch (table->table_scheme[update_attr_num]) {
            case BOOL: {
                struct bool_field *bool_field = tuple->data[update_attr_num];
                bool_field->data = *((int32_t *) new_value);
                break;
            }
            case INT: {
                struct int_field *int_field = tuple->data[update_attr_num];
                int_field->data = *((int32_t *) new_value);
                break;
            }
            case FLOAT: {
                struct float_field *float_field = tuple->data[update_attr_num];
                float_field->data = *((float *) new_value);
                break;
            }
            case STRING: {
                struct string_field *string_field = tuple->data[update_attr_num];
                my_free(string_field->data, string_field->size * sizeof(char));
                string_field->size = strlen((char *) new_value);
                string_field->data = my_malloc(string_field->size * sizeof(char));
                memcpy(string_field->data, new_value, string_field->size);
                break;
            }
        }
    }

    tuple->data_size = data_size(tuple->data, table->num_of_columns, table->table_scheme);
}


void update_table(struct db *db, char *table_name, uint16_t num_of_filters, struct filter **filters,
                  struct update_query *update_query) {
    struct table *table = get_table_by_name(table_name, db->num_of_tables, db->tables);

    if (table == NULL) {
        return;
    }

    struct page *curr_page = table->last_page;

    while (curr_page != NULL) {
        for (size_t i = 0; i < curr_page->num_of_tuples; i++) {
            int is_failed = 0;

            for (size_t j = 0; j < num_of_filters; j++) {
                if (!is_match(curr_page->tuples[i], filters[j], table->num_of_columns, table->columns)) {
                    is_failed = 1;
                    break;
                }
            }

            if (is_failed) {
                continue;
            }

            curr_page->total_free_space_size += data_size(curr_page->tuples[i]->data, table->num_of_columns,
                                                          table->table_scheme);

            struct tuple *new_tuple = curr_page->tuples[i];
            curr_page->tuples[i] = curr_page->tuples[curr_page->num_of_tuples - 1];
            curr_page->num_of_tuples--;
            update_tuple(new_tuple, update_query, table);

            int is_inserted = insert_to_page(curr_page, new_tuple);

            if (!is_inserted) {
                i--;

                if (curr_page != table->last_page) {
                    int is_inserted_last_page = insert_to_page(table->last_page, new_tuple);

                    if (!is_inserted_last_page) {
                        struct page *new_page = create_empty_page(db->last_page_offset, table->last_page->offset,
                                                                  (file_offset) {.offset = NULL_PAGE});

                        db->last_page_offset.offset += PAGE_SIZE;

                        insert_to_page(new_page, new_tuple);

                        table->last_page_offset = new_page->offset;
                        table->last_page->next_page = new_page->offset;
                        write_page(table->last_page, table->num_of_columns, table->table_scheme, db->file);
                        free_page(table->last_page, table->num_of_columns, table->table_scheme);
                        table->last_page = new_page;
                    }
                }
            } else {
                struct tuple *temp = curr_page->tuples[i];
                curr_page->tuples[i] = curr_page->tuples[curr_page->num_of_tuples - 1];
                curr_page->tuples[curr_page->num_of_tuples - 1] = temp;
            }
        }

        int is_curr_page_full = relocate_tuples(table->last_page, curr_page);

        if (!is_curr_page_full) {
            file_offset prev_page = table->last_page->prev_page;
            rewrite_page(db->last_page_offset, table->last_page->offset, db->file);
            check_if_last_page(db->last_page_offset, table->last_page_offset, db);
            free_page(table->last_page, table->num_of_columns, table->table_scheme);

            table->last_page_offset = prev_page;

            if (prev_page.offset != curr_page->offset.offset) {
                table->last_page = read_page(prev_page, table->num_of_columns, table->table_scheme, db->file);
            } else {
                table->last_page = curr_page;
            }

            table->last_page->next_page = (file_offset) {.offset = NULL_PAGE};

            relocate_tuples(table->last_page, curr_page);
        }

        file_offset prev_page = curr_page->prev_page;
        write_page(curr_page, table->num_of_columns, table->table_scheme, db->file);
        if (curr_page != table->last_page) {
            free_page(curr_page, table->num_of_columns, table->table_scheme);
        }
        curr_page = read_page(prev_page, table->num_of_columns, table->table_scheme, db->file);
    }


}

