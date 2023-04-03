#pragma once

#include <bits/types/FILE.h>
#include "my_malloc.h"
#include "is_test.h"


static FILE *file;
static int curr_memory = 0;
static char space = '\n';


static int get_num_of_digits(int number) {
    int num_of_digits = 0;

    while (number > 0) {
        num_of_digits++;
        number /= 10;
    }

    return num_of_digits;
}


static void memory_change() {
    file = fopen("memory_size.txt", "a+");
    int num_of_digits = get_num_of_digits(curr_memory);
    char s[num_of_digits];
    sprintf(s, "%d", curr_memory);
    fwrite(s, num_of_digits, 1, file);
    fwrite(&space, sizeof(char), 1, file);
    fclose(file);
}


void *my_malloc(size_t size) {
    curr_memory += ((int) size);
#if defined MEMORY_TEST
    memory_change();
#endif
    return malloc(size);
}


void my_free(void *ptr, size_t size) {
    curr_memory -= ((int) size);
#if defined MEMORY_TEST
    memory_change();
#endif
    free(ptr);
}

