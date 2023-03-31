#pragma once

#include "table.h"


struct table *create_table(uint64_t name_size, char *name, uint64_t num_of_columns, struct column_list *column_list) {
    struct table *table = my_malloc(sizeof(struct table));
    table->name_size = name_size;
    table->name = name;
    table->num_of_columns = num_of_columns;

    enum data_type *table_scheme = my_malloc(num_of_columns * sizeof(enum data_type));

    for (size_t i = 0; i < num_of_columns; i++) {
        table_scheme[i] = column_list_get(column_list, i)->value->data_type;
    }

    table->column_list = column_list;
    table->table_scheme = table_scheme;

    return table;
}


void free_table(struct table *table) {
    free_page(table->last_page, table->table_scheme, table->num_of_columns);
    my_free(table->table_scheme, table->num_of_columns * sizeof(enum data_type));
    column_list_clear(table->column_list);
    my_free(table->name, table->name_size * sizeof(char));
    my_free(table, sizeof(struct table));
}


struct column *create_column(enum data_type data_type, uint64_t name_size, char *name) {
    struct column *column = my_malloc(sizeof(struct column));
    column->data_type = data_type;
    column->name_size = name_size;
    column->name = name;
    return column;
}


void free_column(struct column *column) {
    my_free(column->name, column->name_size);
    my_free(column, sizeof(struct column));
}

