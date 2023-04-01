#pragma once

#include "filter_type.h"


int is_match(struct tuple *tuple, struct filter *filter) {
    switch (filter->filter_type) {
        case BOOL_EQ:
            if (((struct bool_field *) tuple->data[filter->attribute_num])->data == *((uint32_t *) filter->value))
                return 1;
            else
                return 0;
        case BOOL_NEQ:
            if (((struct bool_field *) tuple->data[filter->attribute_num])->data != *((uint32_t *) filter->value))
                return 1;
            else
                return 0;
        case INT_EQ:
            if (((struct int_field *) tuple->data[filter->attribute_num])->data == *((uint32_t *) filter->value))
                return 1;
            else
                return 0;
        case INT_NEQ:
            if (((struct int_field *) tuple->data[filter->attribute_num])->data != *((uint32_t *) filter->value))
                return 1;
            else
                return 0;
        case FLOAT_EQ:
            if (((struct float_field *) tuple->data[filter->attribute_num])->data == *((float *) filter->value))
                return 1;
            else
                return 0;
        case FLOAT_NEQ:
            if (((struct float_field *) tuple->data[filter->attribute_num])->data != *((float *) filter->value))
                return 1;
            else
                return 0;
        case STR_EQ:
            return !strcmp((char *) ((struct string_field *) tuple->data[filter->attribute_num])->data,
                           (char *) filter->value);
        case STR_NEQ:
            return strcmp((char *) ((struct string_field *) tuple->data[filter->attribute_num])->data,
                          (char *) filter->value);
        case INT_GR:
            if (((struct int_field *) tuple->data[filter->attribute_num])->data > *((int *) filter->value))
                return 1;
            else
                return 0;
        case INT_LESS:
            if (((struct int_field *) tuple->data[filter->attribute_num])->data < *((int *) filter->value))
                return 1;
            else
                return 0;
        case INT_GR_EQ:
            if (((struct int_field *) tuple->data[filter->attribute_num])->data >= *((int *) filter->value))
                return 1;
            else
                return 0;
        case INT_LESS_EQ:
            if (((struct int_field *) tuple->data[filter->attribute_num])->data <= *((int *) filter->value))
                return 1;
            else
                return 0;
        case FLOAT_GR:
            if (((struct float_field *) tuple->data[filter->attribute_num])->data > *((float *) filter->value))
                return 1;
            else
                return 0;
        case FLOAT_LESS:
            if (((struct float_field *) tuple->data[filter->attribute_num])->data < *((float *) filter->value))
                return 1;
            else
                return 0;
        case FLOAT_GR_EQ:
            if (((struct float_field *) tuple->data[filter->attribute_num])->data >= *((float *) filter->value))
                return 1;
            else
                return 0;
        case FLOAT_LESS_EQ:
            if (((struct float_field *) tuple->data[filter->attribute_num])->data <= *((float *) filter->value))
                return 1;
            else
                return 0;
        case STR_LIKE: {
            char *s1 = (char *) ((struct string_field *) tuple->data[filter->attribute_num])->data;
            char *s2 = (char *) filter->value;

            char *result = strstr(s1, s2);

            if (result != NULL)
                return 1;
            else
                return 0;
        }
    }

    return 0;
}

