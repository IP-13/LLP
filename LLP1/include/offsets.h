#pragma once

#include <inttypes.h>


typedef struct file_offset {
    uint64_t offset; // offset in bytes
} file_offset;


typedef struct page_offset {
    uint16_t offset; // offset in bytes
} page_offset;

