#pragma once

#include <inttypes.h>
#include <stdio.h>
#include "my_malloc.h"
#include <string.h>

struct vector {
    void **data;
    uint64_t capacity;
    uint64_t size;
    size_t element_size;
};

struct vector *create_vector(uint64_t capacity, size_t element_size);

void vector_push(struct vector *vector, void *data);

int vector_remove(struct vector *vector, size_t index);

void *vector_get(struct vector *vector, size_t index);

void vector_resize(struct vector *vector, uint64_t new_capacity);

void vector_clear(struct vector *vector);