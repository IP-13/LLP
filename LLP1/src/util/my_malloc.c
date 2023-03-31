#pragma once

#include "../../include/my_malloc.h"

void *my_malloc(size_t size) {
    return malloc(size);
}


void my_free(void *ptr, size_t size) {
    free(ptr);
}

void *my_realloc(void *ptr, size_t size, size_t realloc_size) {
    realloc(ptr, size);
}