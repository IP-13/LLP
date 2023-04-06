#pragma once

#include "table.h"


struct table *create_table(file_offset last_page_offset, uint64_t name_size, char *name,
                           uint64_t num_of_columns, struct column **columns) {
    struct table *table = my_malloc(sizeof(struct table));
    table->last_page_offset = last_page_offset;
    table->name_size = name_size;
    table->name = name;
    table->num_of_columns = num_of_columns;
    table->columns = columns;

    enum data_type *table_scheme = my_malloc(num_of_columns * sizeof(enum data_type));

    for (size_t i = 0; i < num_of_columns; i++) {
        table_scheme[i] = columns[i]->data_type;
    }

    table->table_scheme = table_scheme;

    return table;
}


void free_table(struct table *table) {
    free_page(table->last_page, table->num_of_columns, table->table_scheme);
    my_free(table->table_scheme, table->num_of_columns * sizeof(enum data_type));

    for (size_t i = 0; i < table->num_of_columns; i++) {
        free_column(table->columns[i]);
    }

    my_free(table->columns, table->num_of_columns * sizeof(struct column *));
    my_free(table->name, table->name_size * sizeof(char));
    my_free(table, sizeof(struct table));
}


struct table *get_table_by_name(char *name, uint64_t num_of_tables, struct table **tables) {
    for (size_t i = 0; i < num_of_tables; i++) {
        if (!strcmp(tables[i]->name, name)) {
            return tables[i];
        }
    }

    return NULL;
}