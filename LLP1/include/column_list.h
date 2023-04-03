#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "my_malloc.h"
#include "structures.h"


void clear_column_node(struct column_list *column_list);

struct column_list *create_column_list();

void column_list_push(struct column_list *column_list, struct column *value);

int column_list_remove(struct column_list **column_list, size_t index);

struct column_list *column_list_get(struct column_list *column_list, size_t index);

void column_list_clear(struct column_list *column_list);

