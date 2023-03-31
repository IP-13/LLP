#pragma once

#include "vector.h"

struct vector *create_vector(uint64_t capacity, size_t element_size) {
    struct vector *vector = my_malloc(sizeof(struct vector));
    vector->capacity = capacity;
    vector->size = 0;
    vector->element_size = element_size;
    vector->data = (void **) my_malloc(vector->capacity * sizeof(void *));

    return vector;
}


void vector_push(struct vector *vector, void *data) {
    if (vector->capacity == vector->size) {
        vector_resize(vector, 2 * vector->capacity);
    }

    vector->data[vector->size] = my_malloc(vector->element_size);
    memcpy(vector->data[vector->size], data, vector->element_size);

    vector->size++;
}


int vector_remove(struct vector *vector, size_t index) {
    if (index >= vector->size) {
        return 0;
    }

    my_free(vector->data[index], sizeof(vector->data[index]));

    for (size_t i = index; i < vector->size - 1; i++) {
        vector->data[i] = vector->data[i + 1];
    }

    if (vector->size >= 10 && vector->size < vector->capacity / 4) {
        vector_resize(vector, vector->capacity / 2);
    }

    vector->size--;

    return 1;
}


void *vector_get(struct vector *vector, size_t index) {
    if (index >= vector->size) {
        return NULL;
    }

    return vector->data[index];
}


void vector_resize(struct vector *vector, uint64_t new_capacity) {
    uint64_t old_capacity = vector->capacity;
    vector->capacity = new_capacity;
    vector->data = my_realloc(vector->data, new_capacity * sizeof(void *), new_capacity - old_capacity);
}


void vector_clear(struct vector *vector) {
    for (size_t i = 0; i < vector->size; i++) {
        my_free(vector->data[i], vector->element_size);
    }

    my_free(vector->data, vector->capacity * sizeof(void *));

    my_free(vector, sizeof(struct vector));
}