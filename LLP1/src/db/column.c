#pragma once

#include "column.h"


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


int64_t get_column_index(char *name, uint64_t num_of_columns, struct column **columns) {
    if (name == NULL || columns == NULL || num_of_columns == 0) {
        return -1;
    }

    for (int64_t i = 0; i < num_of_columns; i++) {
        if(!strcmp(columns[i]->name, name)) {
            return i;
        }
    }

    return -1;
}


struct column *get_column_by_name(char *name, uint64_t num_of_columns, struct column **columns) {
    if (name == NULL || columns == NULL || num_of_columns == 0) {
        return NULL;
    }

    for (size_t i = 0; i < num_of_columns; i++) {
        if (!strcmp(columns[i]->name, name)) {
            return columns[i];
        }
    }

    return NULL;
}

