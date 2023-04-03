#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "my_malloc.h"
#include "structures.h"


struct table_list *create_table_list();

void table_list_push(struct table_list *table_list, struct table *value);

int table_list_remove(struct table_list **table_list, size_t index);

struct table_list *table_list_get(struct table_list *table_list, int32_t index);

void table_list_clear(struct table_list *table_list);

int get_table_index_by_name(struct table_list *table_list, const char *name);

