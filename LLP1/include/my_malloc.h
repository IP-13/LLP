#pragma once

#include <stdlib.h>


void *my_malloc(size_t size);

void my_free(void *ptr, size_t size);

void *my_realloc(void *ptr, size_t size, size_t realloc_size);