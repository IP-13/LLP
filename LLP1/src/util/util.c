#pragma once

#include "util.h"


void print_tuple(struct tuple *tuple, uint32_t num_of_columns, const enum data_type *table_scheme) {
    for (size_t i = 0; i < num_of_columns; i++) {
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

void free_filter_value(void *value, enum filter_cond filter_cond, enum filter_type filter_type) {
    if (filter_type == CONST) {
        my_free(value, strlen(value));
        return;
    }

    switch (filter_cond) {
        case BOOL_EQ:
        case BOOL_NEQ:
            my_free(value, sizeof(int32_t));
            break;
        case INT_EQ:
        case INT_NEQ:
        case INT_GR:
        case INT_LESS:
        case INT_GR_EQ:
        case INT_LESS_EQ:
            my_free(value, sizeof(int32_t));
            break;
        case FLOAT_EQ:
        case FLOAT_NEQ:
        case FLOAT_GR:
        case FLOAT_LESS:
        case FLOAT_GR_EQ:
        case FLOAT_LESS_EQ:
            my_free(value, sizeof(float));
            break;
        case STR_EQ:
        case STR_NEQ:
        case STR_LIKE: {
            size_t size = strlen(value);
            my_free(value, size);
            break;
        }
    }
}

void free_filters(uint16_t num_of_filters, struct filter **filters) {
    for (size_t i = 0; i < num_of_filters; i++) {
        free_filter_value(filters[i]->value, filters[i]->filter_cond, filters[i]->filter_type);
        my_free(filters[i], sizeof(struct filter));
    }

    my_free(filters, num_of_filters * sizeof(struct filter *));
}

