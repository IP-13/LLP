#pragma once

#include "filters.h"


int32_t is_match(struct tuple *tuple, struct filter *filter, uint64_t num_of_columns, struct column **columns) {
    switch (filter->filter_cond) {
        case BOOL_EQ: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct bool_field *curr_field = tuple->data[curr_field_index];
            struct bool_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return curr_field->data == *((int32_t *) filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return curr_field->data == compare_field->data;
            } else {
                return 0;
            }
        }
        case BOOL_NEQ: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct bool_field *curr_field = tuple->data[curr_field_index];
            struct bool_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return curr_field->data != *((int32_t *) filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return curr_field->data != compare_field->data;
            } else {
                return 0;
            }
        }
        case INT_EQ: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct int_field *curr_field = tuple->data[curr_field_index];
            struct int_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return curr_field->data == *((int32_t *) filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return curr_field->data == compare_field->data;
            } else {
                return 0;
            }
        }
        case INT_NEQ: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct int_field *curr_field = tuple->data[curr_field_index];
            struct int_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return curr_field->data != *((int32_t *) filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return curr_field->data != compare_field->data;
            } else {
                return 0;
            }
        }
        case FLOAT_EQ: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct float_field *curr_field = tuple->data[curr_field_index];
            struct float_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return curr_field->data == *((float *) filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return curr_field->data == compare_field->data;
            } else {
                return 0;
            }
        }
        case FLOAT_NEQ: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct float_field *curr_field = tuple->data[curr_field_index];
            struct float_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return curr_field->data != *((float *) filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return curr_field->data != compare_field->data;
            } else {
                return 0;
            }
        }
        case STR_EQ: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct string_field *curr_field = tuple->data[curr_field_index];
            struct string_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return !strcmp(curr_field->data, filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return !strcmp(curr_field->data, compare_field->data);
            } else {
                return 0;
            }
        }
        case STR_NEQ: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct string_field *curr_field = tuple->data[curr_field_index];
            struct string_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return strcmp(curr_field->data, filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return strcmp(curr_field->data, compare_field->data);
            } else {
                return 0;
            }
        }
        case INT_GR: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct int_field *curr_field = tuple->data[curr_field_index];
            struct int_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return curr_field->data > *((int32_t *) filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return curr_field->data > compare_field->data;
            } else {
                return 0;
            }
        }
        case INT_LESS: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct int_field *curr_field = tuple->data[curr_field_index];
            struct int_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return curr_field->data < *((int32_t *) filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return curr_field->data < compare_field->data;
            } else {
                return 0;
            }
        }
        case INT_GR_EQ: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct int_field *curr_field = tuple->data[curr_field_index];
            struct int_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return curr_field->data >= *((int32_t *) filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return curr_field->data >= compare_field->data;
            } else {
                return 0;
            }
        }
        case INT_LESS_EQ: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct int_field *curr_field = tuple->data[curr_field_index];
            struct int_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return curr_field->data <= *((int32_t *) filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return curr_field->data <= compare_field->data;
            } else {
                return 0;
            }
        }
        case FLOAT_GR: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct float_field *curr_field = tuple->data[curr_field_index];
            struct float_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return curr_field->data > *((float *) filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return curr_field->data > compare_field->data;
            } else {
                return 0;
            }
        }
        case FLOAT_LESS: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct float_field *curr_field = tuple->data[curr_field_index];
            struct float_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return curr_field->data < *((float *) filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return curr_field->data < compare_field->data;
            } else {
                return 0;
            }
        }
        case FLOAT_GR_EQ: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct float_field *curr_field = tuple->data[curr_field_index];
            struct float_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return curr_field->data >= *((float *) filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return curr_field->data >= compare_field->data;
            } else {
                return 0;
            }
        }
        case FLOAT_LESS_EQ: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct float_field *curr_field = tuple->data[curr_field_index];
            struct float_field *compare_field = NULL;

            if (filter->filter_type == CONST) {
                return curr_field->data <= *((float *) filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                return curr_field->data <= compare_field->data;
            } else {
                return 0;
            }
        }
        case STR_LIKE: {
            int64_t curr_field_index = get_column_index(filter->col_name, num_of_columns, columns);

            if (curr_field_index == -1) {
                return 0;
            }

            struct string_field *curr_field = tuple->data[curr_field_index];
            struct string_field *compare_field = NULL;
            char *result = NULL;

            if (filter->filter_type == CONST) {
                result = strstr(curr_field->data, filter->value);
            } else if (filter->filter_type == REFERENCE) {
                compare_field = tuple->data[get_column_index(filter->value, num_of_columns, columns)];
                result = strstr(curr_field->data, compare_field->data);
            } else {
                return 0;
            }

            if (result != NULL)
                return 1;
            else
                return 0;
        }
        default: {
            return 0;
        }
    }
}

