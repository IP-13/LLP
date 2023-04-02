#pragma once

#include "db.h"
#include "table.h"
#include "is_test.h"

struct db *create_db(char *filename) {
    FILE *file = fopen(filename, "w+");
    file_offset last_page_offset = (file_offset) {.offset = 0};
    struct table_list *table_list = create_table_list();

    struct db *db = my_malloc(sizeof(struct db));

    db->name = filename;
    db->file = file;
    db->last_page_offset = last_page_offset;
    db->table_list = table_list;

    return db;
}


struct column_list *read_column_list(FILE *file, uint64_t num_of_columns) {
    struct column_list *column_list = create_column_list();

    for (size_t i = 0; i < num_of_columns; i++) {
        struct column *column = my_malloc(sizeof(struct column));

        enum data_type data_type;
        fread(&data_type, sizeof(data_type), 1, file);

        uint64_t name_size;
        fread(&name_size, sizeof(name_size), 1, file);

        char *name = my_malloc(sizeof(char) * name_size);
        fread(name, sizeof(char), name_size, file);

        column->data_type = data_type;
        column->name_size = name_size;
        column->name = name;

        column_list_push(column_list, column);
    }

    return column_list;
}


struct table_list *read_table_list(FILE *file, uint64_t num_of_tables, file_offset db_last_page_offset) {
    struct table_list *table_list = create_table_list();

    fseek(file, db_last_page_offset.offset, SEEK_SET);

    for (size_t i = 0; i < num_of_tables; i++) {
        struct table *table = my_malloc(sizeof(struct table));

        file_offset last_page_offset;
        fread(&last_page_offset.offset, sizeof(last_page_offset.offset), 1, file);

        uint64_t name_size;
        fread(&name_size, sizeof(name_size), 1, file);

        char *name = my_malloc(sizeof(char) * name_size);
        fread(name, sizeof(char), name_size, file);

        uint64_t num_of_columns;
        fread(&num_of_columns, sizeof(num_of_columns), 1, file);

        struct column_list *column_list = read_column_list(file, num_of_columns);

        enum data_type *table_scheme = my_malloc(num_of_columns * sizeof(enum data_type));

        struct column_list *temp = column_list;

        for (size_t j = 0; j < num_of_columns; j++) {
            table_scheme[j] = temp->value->data_type;
            temp = temp->next;
        }

        table->last_page_offset = last_page_offset;
        table->name_size = name_size;
        table->name = name;
        table->num_of_columns = num_of_columns;
        table->column_list = column_list;
        table->table_scheme = table_scheme;

        table_list_push(table_list, table);
    }

    return table_list;
}


struct db *open_db(char *filename) {
    FILE *file = fopen(filename, "r+");

    if (file == NULL) {
        return create_db(filename);
    }

    struct db *db = my_malloc(sizeof(struct db));

    uint64_t num_of_tables;
    fseek(file, -8, SEEK_END);
    fread(&num_of_tables, sizeof(uint64_t), 1, file);

    file_offset last_page_offset;
    fseek(file, -16, SEEK_END);
    fread(&(last_page_offset.offset), sizeof(last_page_offset.offset), 1, file);

    db->name = filename;
    db->file = file;
    db->num_of_tables = num_of_tables;
    db->last_page_offset = last_page_offset;
    db->table_list = read_table_list(file, num_of_tables, last_page_offset);

    for (size_t i = 0; i < db->num_of_tables; i++) {
        struct table *table = table_list_get(db->table_list, i)->value;
        struct page *page = read_page(file, table->last_page_offset, table->table_scheme, table->num_of_columns);
        table->last_page = page;
    }

    return db;
}


void write_columns(struct column_list *column_list, FILE *file, size_t num_of_columns) {
    for (size_t i = 0; i < num_of_columns; i++) {
        struct column *column = column_list_get(column_list, i)->value;
        fwrite(&column->data_type, sizeof(column->data_type), 1, file);
        fwrite(&column->name_size, sizeof(column->name_size), 1, file);
        fwrite(column->name, sizeof(char), column->name_size, file);
    }
}


void write_tables(struct table_list *table_list, FILE *file, size_t num_of_tables) {
    for (size_t i = 0; i < num_of_tables; i++) {
        struct table *table = table_list_get(table_list, i)->value;
        fwrite(&table->last_page_offset.offset, sizeof(table->last_page_offset.offset), 1, file);
        fwrite(&table->name_size, sizeof(table->name_size), 1, file);
        fwrite(table->name, sizeof(char), table->name_size, file);
        fwrite(&table->num_of_columns, sizeof(table->num_of_columns), 1, file);
        write_columns(table->column_list, file, table->num_of_columns);
    }

    for (size_t i = 0; i < num_of_tables; i++) {
        struct table *table = table_list_get(table_list, i)->value;
        write_page(table->last_page, table->num_of_columns, table->table_scheme, file);
    }

    table_list_clear(table_list);
}


void close_db(struct db *db) {
    FILE *file = db->file;
    fseek(file, db->last_page_offset.offset, SEEK_SET);
    write_tables(db->table_list, file, db->num_of_tables);
    fseek(file, 0, SEEK_END);
    fwrite(&(db->last_page_offset.offset), sizeof(db->last_page_offset.offset), 1, file);
    fwrite(&(db->num_of_tables), sizeof(db->num_of_tables), 1, file);
    my_free(db->name, TABLE_NAME_SIZE * sizeof(char));
    my_free(db, sizeof(struct db));
    fclose(file);
}


int add_table(struct db *db, struct table *table) {
    if (db->table_list == NULL) {
        db->table_list = create_table_list();
    }


    if (get_table_index_by_name(db->table_list, table->name) != -1) {
        return 0;
    }

    table->last_page_offset.offset = db->last_page_offset.offset;
    table->last_page = create_page(table->last_page_offset,
                                   (file_offset) {.offset = NULL_PAGE},
                                   (file_offset) {.offset=NULL_PAGE});

    db->last_page_offset.offset += PAGE_SIZE;

    write_page(table->last_page, table->num_of_columns, table->table_scheme, db->file);

    table_list_push(db->table_list, table);
    db->num_of_tables++;

    return 1;
}

// checks if last db page is last page of any table
void check_if_last_page(struct db *db, file_offset last_db_page_offset, file_offset new_offset) {
    for (size_t i = 0; i < db->num_of_tables; i++) {
        struct table *curr_table = table_list_get(db->table_list, i)->value;
        if (curr_table->last_page_offset.offset == last_db_page_offset.offset) {
            curr_table->last_page_offset.offset = new_offset.offset;
            curr_table->last_page->offset.offset = new_offset.offset;
        }
    }
}


int delete_table(struct db *db, char *name) {
    struct table *table = table_list_get(db->table_list, get_table_index_by_name(db->table_list, name))->value;

    if (table == NULL) {
        return -1;
    }

    file_offset delete_page_offset = table->last_page_offset;

    while (delete_page_offset.offset != NULL_PAGE) {
        struct page *curr_page_info = read_page_info(db->file, delete_page_offset);
        db->last_page_offset.offset -= PAGE_SIZE;
        file_offset last_page_offset = db->last_page_offset;

        if (delete_page_offset.offset != last_page_offset.offset) {
            rewrite_page(db->file, last_page_offset, delete_page_offset);
            check_if_last_page(db, last_page_offset, delete_page_offset);
        }

        delete_page_offset.offset = curr_page_info->prev_page.offset;

        my_free(curr_page_info, sizeof(struct page));
    }


    table_list_remove(&db->table_list, get_table_index_by_name(db->table_list, name));
    db->num_of_tables--;

    if (db->num_of_tables == 0) {
        db->table_list = create_table_list();
    }

    return 1;
}


void insert_to_table(struct db *db, char *table_name, struct tuple *tuple) {
    struct table *table = table_list_get(db->table_list, get_table_index_by_name(db->table_list, table_name))->value;

    if (table == NULL) {
        return;
    }

    int is_inserted = insert_to_page(table->last_page, tuple);

    if (!is_inserted) {
        file_offset prev_page = table->last_page->offset;
        struct page *new_last_page = create_page(db->last_page_offset, prev_page, (file_offset) {.offset = NULL_PAGE});

        table->last_page->next_page = new_last_page->offset;

        write_page(table->last_page, table->num_of_columns, table->table_scheme, db->file);

        free_page(table->last_page, table->table_scheme, table->num_of_columns);

        table->last_page_offset = new_last_page->offset;
        table->last_page = new_last_page;

        db->last_page_offset.offset += PAGE_SIZE;

        insert_to_page(table->last_page, tuple);
    }
}

// test function
// TODO change to server
void print_tuple(struct tuple *tuple, uint32_t num_of_attributes, const enum data_type *table_scheme) {
    for (size_t i = 0; i < num_of_attributes; i++) {
        switch (table_scheme[i]) {
            case BOOL: {
                if (!((struct bool_field *) tuple->data[i])->data) {
                    printf("%-20s | ", "FALSE");
                } else {
                    printf("%-20s | ", "TRUE");
                }
                break;
            }
            case INT: {
                printf("%-20"PRId32" | ", ((struct int_field *) tuple->data[i])->data);
                break;
            }
            case FLOAT: {
                printf("%-20f | ", ((struct float_field *) tuple->data[i])->data);
                break;
            }
            case STRING: {
                struct string_field *string_field = tuple->data[i];
                printf("%-20s | ", string_field->data);
                break;
            }
            default:
                printf("Something went wrong\n");
        }
    }
}


void handle_page_select(struct page_select *page_select, enum data_type *table_scheme, uint64_t num_of_attributes) {
#ifndef TEST
    for (size_t i = 0; i < page_select->num_of_tuples; i++) {
        print_tuple(page_select->tuples[i], num_of_attributes, table_scheme);
        printf("\n");
    }
#endif
    my_free(page_select->tuples, MAX_NUM_OF_TUPLES * sizeof(struct tuple *));
    my_free(page_select, sizeof(struct page_select));
}


void select_from_table(struct db *db, char *table_name, uint16_t num_of_filter, struct filter **filters) {
    struct table_list *table_list = table_list_get(db->table_list, get_table_index_by_name(db->table_list, table_name));

    if (table_list == NULL) {
        return;
    }

    struct table *table = table_list->value;

    if (table == NULL) {
        return;
    }

    struct page *curr_page = table->last_page;

    while (curr_page->prev_page.offset != NULL_PAGE) {
        struct page_select *page_select = select_from_page(curr_page, num_of_filter, filters);
        handle_page_select(page_select, table->table_scheme, table->num_of_columns);

        file_offset prev_page = curr_page->prev_page;

        if (curr_page != table->last_page) {
            free_page(curr_page, table->table_scheme, table->num_of_columns);
        }
        curr_page = read_page(db->file, prev_page, table->table_scheme, table->num_of_columns);
    }

    struct page_select *page_select = select_from_page(curr_page, num_of_filter, filters);
    handle_page_select(page_select, table->table_scheme, table->num_of_columns);

    if (curr_page != table->last_page) {
        free_page(curr_page, table->table_scheme, table->num_of_columns);
    }
}


void relocate_tuples(struct page *from, struct page *to, struct table *table, struct db *db) {
    while (from->num_of_tuples > 0) {
        struct tuple *last_tuple = from->tuples[from->num_of_tuples - 1];

        int is_inserted = insert_to_page(to, last_tuple);

        if (!is_inserted) {
            break;
        }

        from->num_of_tuples--;
        from->total_free_space_size += last_tuple->size;
    }

    if (from->num_of_tuples == 0) {
        db->last_page_offset.offset -= PAGE_SIZE;

        check_if_last_page(db, db->last_page_offset, from->offset);

        rewrite_page(db->file, db->last_page_offset, from->offset);

        file_offset prev_page = from->prev_page;

        free_page(from, table->table_scheme, table->num_of_columns);

        if (prev_page.offset == to->offset.offset) {
            write_page(to, table->num_of_columns, table->table_scheme, db->file);
        }

        from = read_page(db->file, prev_page, table->table_scheme, table->num_of_columns);

        table->last_page = from;
        table->last_page_offset.offset = from->offset.offset;
    }

    if (from->offset.offset == to->offset.offset) {
        return;
    }

    while (from->num_of_tuples > 0) {
        struct tuple *last_tuple = from->tuples[from->num_of_tuples - 1];

        int is_inserted = insert_to_page(to, last_tuple);

        if (!is_inserted) {
            break;
        }

        from->num_of_tuples--;
        from->total_free_space_size += last_tuple->size;
    }
}


void delete_from_table(struct db *db, char *table_name, uint16_t num_of_filters, struct filter **filters) {
    struct table_list *table_list = table_list_get(db->table_list, get_table_index_by_name(db->table_list, table_name));

    if (table_list == NULL) {
        return;
    }

    struct table *table = table_list->value;

    if (table == NULL) {
        return;
    }

    struct page *curr_page = table->last_page;

    while (curr_page != NULL) {
        delete_from_page(curr_page, num_of_filters, filters, table->table_scheme, table->num_of_columns);

        if (curr_page == table->last_page) {
            curr_page = read_page(db->file, table->last_page->prev_page, table->table_scheme, table->num_of_columns);
            continue;
        }

        relocate_tuples(table->last_page, curr_page, table, db);

        file_offset prev_page = curr_page->prev_page;

        write_page(curr_page, table->num_of_columns, table->table_scheme, db->file);
        free_page(curr_page, table->table_scheme, table->num_of_columns);
        curr_page = read_page(db->file, prev_page, table->table_scheme, table->num_of_columns);
    }
}


void join_table(struct db *db, char *table1_name, char *table2_name, uint16_t num_of_filters1, struct filter **filters1,
                uint16_t num_of_filters2, struct filter **filters2) {
    struct table_list *table_list1 = table_list_get(db->table_list,
                                                    get_table_index_by_name(db->table_list, table1_name));

    if (table_list1 == NULL) {
        return;
    }

    struct table *table1 = table_list1->value;

    if (table1 == NULL) {
        return;
    }


    struct table_list *table_list2 = table_list_get(db->table_list,
                                                    get_table_index_by_name(db->table_list, table2_name));

    if (table_list2 == NULL) {
        return;
    }

    struct table *table2 = table_list2->value;

    if (table2 == NULL) {
        return;
    }

    struct page *curr_page1 = table1->last_page;

    while (curr_page1->prev_page.offset != NULL_PAGE) {
        struct page_select *page_select1 = select_from_page(curr_page1, num_of_filters1, filters1);

        struct page *curr_page2 = table2->last_page;

        while (curr_page2->prev_page.offset != NULL_PAGE) {
            struct page_select *page_select2 = select_from_page(curr_page2, num_of_filters2, filters2);

            for (size_t i = 0; i < page_select1->num_of_tuples; i++) {
                for (size_t j = 0; j < page_select2->num_of_tuples; j++) {
                    print_tuple(page_select1->tuples[i], table1->num_of_columns, table1->table_scheme);
                    print_tuple(page_select2->tuples[j], table2->num_of_columns, table2->table_scheme);
                    printf("\n");
                }
            }

            if (curr_page2 != table2->last_page) {
                free_page(curr_page2, table2->table_scheme, table2->num_of_columns);
            }

            curr_page2 = read_page(db->file, curr_page2->prev_page, table2->table_scheme, table2->num_of_columns);
        }

        struct page_select *page_select2 = select_from_page(curr_page2, num_of_filters2, filters2);

        for (size_t i = 0; i < page_select1->num_of_tuples; i++) {
            for (size_t j = 0; j < page_select2->num_of_tuples; j++) {
                print_tuple(page_select1->tuples[i], table1->num_of_columns, table1->table_scheme);
                print_tuple(page_select2->tuples[j], table2->num_of_columns, table2->table_scheme);
                printf("\n");
            }
        }

        if (curr_page2 != table2->last_page) {
            free_page(curr_page2, table2->table_scheme, table2->num_of_columns);
        }


        if (curr_page1 != table1->last_page) {
            free_page(curr_page1, table1->table_scheme, table1->num_of_columns);
        }

        curr_page1 = read_page(db->file, curr_page1->prev_page, table1->table_scheme, table1->num_of_columns);
    }


    struct page_select *page_select1 = select_from_page(curr_page1, num_of_filters1, filters1);


    struct page *curr_page2 = table2->last_page;

    while (curr_page2->prev_page.offset != NULL_PAGE) {
        struct page_select *page_select2 = select_from_page(curr_page2, num_of_filters2, filters2);

        for (size_t i = 0; i < page_select1->num_of_tuples; i++) {
            for (size_t j = 0; j < page_select2->num_of_tuples; j++) {
                print_tuple(page_select1->tuples[i], table1->num_of_columns, table1->table_scheme);
                print_tuple(page_select2->tuples[j], table2->num_of_columns, table2->table_scheme);
                printf("\n");
            }
        }

        if (curr_page2 != table2->last_page) {
            free_page(curr_page2, table2->table_scheme, table2->num_of_columns);
        }

        curr_page2 = read_page(db->file, curr_page2->prev_page, table2->table_scheme, table2->num_of_columns);
    }

    struct page_select *page_select2 = select_from_page(curr_page2, num_of_filters2, filters2);

    for (size_t i = 0; i < page_select1->num_of_tuples; i++) {
        for (size_t j = 0; j < page_select2->num_of_tuples; j++) {
            print_tuple(page_select1->tuples[i], table1->num_of_columns, table1->table_scheme);
            print_tuple(page_select2->tuples[j], table2->num_of_columns, table2->table_scheme);
            printf("\n");
        }
    }

    if (curr_page2 != table2->last_page) {
        free_page(curr_page2, table2->table_scheme, table2->num_of_columns);
    }


    if (curr_page1 != table1->last_page) {
        free_page(curr_page1, table1->table_scheme, table1->num_of_columns);
    }
}


void update_table(struct db *db, char *table_name, uint16_t num_of_filters, struct filter **filters,
                  struct update_query *update_query) {
    struct table_list *table_list = table_list_get(db->table_list, get_table_index_by_name(db->table_list, table_name));

    if (table_list == NULL) {
        return;
    }

    struct table *table = table_list->value;

    if (table == NULL) {
        return;
    }

    struct page *curr_page = table->last_page;

    while (curr_page != NULL) {
        page_update(db, table, curr_page, num_of_filters, filters, update_query);
        file_offset prev_page = curr_page->prev_page;

        if (curr_page != table->last_page) {
            write_page(curr_page, table->num_of_columns, table->table_scheme, db->file);
            free_page(curr_page, table->table_scheme, table->num_of_columns);
        }

        curr_page = read_page(db->file, prev_page, table->table_scheme, table->num_of_columns);
    }
}

