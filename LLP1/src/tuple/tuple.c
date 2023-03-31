#pragma once

#include "tuple.h"


struct tuple *read_tuple(uint64_t num_of_attributes, const enum data_type *table_scheme, FILE *file) {
    page_offset start;
    fread(&start, sizeof(page_offset), 1, file);

    void **data = (void **) my_malloc(sizeof(void *));

    for (size_t i = 0; i < num_of_attributes; i++) {
        switch (table_scheme[i]) {
            case BOOL:
                data[i] = my_malloc(sizeof(struct bool_field));
                fread(data[i], sizeof(int32_t), 1, file);
                break;
            case INT:
                data[i] = my_malloc(sizeof(struct int_field));
                fread(data[i], sizeof(int32_t), 1, file);
                break;
            case FLOAT:
                data[i] = my_malloc(sizeof(struct float_field));
                fread(data[i], sizeof(float), 1, file);
                break;
            case STRING:
                data[i] = my_malloc(sizeof(struct string_field));
                fread(&((struct string_field *) data[i])->size, sizeof(uint16_t), 1, file);
                ((struct string_field *) data[i])->data = my_malloc(((struct string_field *) data[i])->size);
                fread(((struct string_field *) data[i])->data, ((struct string_field *) data[i])->size, 1, file);
                break;
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
            case BOOL:
                fwrite(tuple->data[i], sizeof(struct bool_field), 1, file);
                break;
            case INT:
                fwrite(tuple->data[i], sizeof(struct int_field), 1, file);
                break;
            case FLOAT:
                fwrite(tuple->data[i], sizeof(struct float_field), 1, file);
                break;
            case STRING:
                fwrite(&((struct string_field *) tuple->data[i])->size, sizeof(uint16_t), 1, file);
                fwrite(((struct string_field *) tuple->data[i])->data, sizeof(char),
                       ((struct string_field *) tuple->data[i])->size, file);
                break;
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

