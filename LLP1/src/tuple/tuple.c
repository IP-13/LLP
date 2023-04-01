#pragma once

#include "tuple.h"


struct tuple *read_tuple(uint64_t num_of_attributes, const enum data_type *table_scheme, FILE *file) {
    page_offset start;
    fread(&start, sizeof(page_offset), 1, file);

    void **data = (void **) my_malloc(num_of_attributes * sizeof(void *));

    for (size_t i = 0; i < num_of_attributes; i++) {
        switch (table_scheme[i]) {
            case BOOL: {
                struct bool_field *bool_field = my_malloc(sizeof(struct bool_field));
                fread(&bool_field->data, sizeof(int32_t), 1, file);
                data[i] = bool_field;
                break;
            }
            case INT: {
                struct int_field *int_field = my_malloc(sizeof(struct int_field));
                fread(&int_field->data, sizeof(int32_t), 1, file);
                data[i] = int_field;
                break;
            }
            case FLOAT: {
                struct float_field *float_field = my_malloc(sizeof(struct float_field));
                fread(&float_field->data, sizeof(float), 1, file);
                data[i] = float_field;
                break;
            }
            case STRING: {
                struct string_field *string_field = my_malloc(sizeof(struct string_field));
                fread(&string_field->size, sizeof(uint16_t), 1, file);
                string_field->data = my_malloc(string_field->size * sizeof(char));
                fread(string_field->data, sizeof(char), string_field->size, file);
                data[i] = string_field;
                break;
            }
            default:
                return NULL;
        }
    }

    struct tuple *tuple = create_tuple(start, data, num_of_attributes, table_scheme);

    return tuple;
}


int write_tuple(struct tuple *tuple, uint64_t num_of_attributes, const enum data_type *table_scheme, FILE *file) {
    fwrite(&tuple->start.offset, sizeof(tuple->start.offset), 1, file);

    for (size_t i = 0; i < num_of_attributes; i++) {
        switch (table_scheme[i]) {
            case BOOL: {
                struct bool_field *bool_field = tuple->data[i];
                fwrite(&bool_field->data, sizeof(int32_t), 1, file);
                break;
            }
            case INT: {
                struct int_field *int_field = tuple->data[i];
                fwrite(&int_field->data, sizeof(int32_t), 1, file);
                break;
            }
            case FLOAT: {
                struct float_field *float_field = tuple->data[i];
                fwrite(&float_field->data, sizeof(float), 1, file);
                break;
            }
            case STRING: {
                struct string_field *string_field = tuple->data[i];
                fwrite(&string_field->size, sizeof(uint16_t), 1, file);
                fwrite(string_field->data, sizeof(char),string_field->size, file);
                break;
            }
            default:
                return 0;
        }
    }

    return 1;
}


struct tuple *create_tuple(page_offset start, void **data,
                           uint64_t num_of_attributes, const enum data_type *table_scheme) {
    struct tuple *tuple = my_malloc(sizeof(struct tuple));
    tuple->start = start;
    tuple->size = data_size(data, table_scheme, num_of_attributes) + sizeof(page_offset);
    tuple->data = data;
    return tuple;
}


void free_tuple(struct tuple *tuple, const enum data_type *table_scheme, uint64_t num_of_attributes) {
    for (size_t i = 0; i < num_of_attributes; i++) {
        switch (table_scheme[i]) {
            case BOOL:
                my_free(tuple->data[i], sizeof(struct bool_field));
                break;
            case INT:
                my_free(tuple->data[i], sizeof(struct int_field));
                break;
            case FLOAT:
                my_free(tuple->data[i], sizeof(struct float_field));
                break;
            case STRING:
                my_free(((struct string_field *) tuple->data[i])->data, ((struct string_field *) tuple->data[i])->size);
                my_free(tuple->data[i], sizeof(struct string_field));
                break;
        }
    }

    my_free(tuple->data, num_of_attributes * sizeof(void *));
    my_free(tuple, sizeof(struct tuple));
}


uint16_t data_size(void **data, const enum data_type *table_scheme, uint64_t num_of_attributes) {
    size_t data_size = 0;
    for (size_t i = 0; i < num_of_attributes; i++) {
        switch (table_scheme[i]) {
            case BOOL:
                data_size += sizeof(struct bool_field);
                break;
            case INT:
                data_size += sizeof(struct int_field);
                break;
            case FLOAT:
                data_size += sizeof(struct float_field);
                break;
            case STRING:
                data_size += sizeof(uint16_t);
                data_size += (((struct string_field *) data[i])->size * sizeof(char));
                break;
        }
    }

    return data_size;
}

