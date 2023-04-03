#pragma once

#include "table_list.h"
#include "column_list.h"


void clear_table_node(struct table_list *table_list) {
    free_table(table_list->value);
    my_free(table_list, sizeof(struct table_list));
}


struct table_list *create_table_list() {
    struct table_list *table_list = my_malloc(sizeof(struct table_list));
    table_list->value = NULL;
    table_list->next = NULL;
    return table_list;
}


void table_list_push(struct table_list *table_list, struct table *value) {
    if (table_list->value == NULL) {
        table_list->value = value;
    } else {
        while (table_list->next != NULL) {
            table_list = table_list->next;
        }
        table_list->next = create_table_list();
        table_list->next->value = value;
    }
}


int table_list_remove(struct table_list **table_list_p, size_t index) {
    struct table_list *table_list = *table_list_p;

    if (table_list == NULL || table_list->value == NULL) {
        return 0;
    }

    if (index != 0) {
        for (size_t i = 0; i < index - 1; i++) {
            if (table_list->next != NULL) {
                table_list = table_list->next;
            } else {
                return 0;
            }
        }

        if (table_list->next == NULL) {
            return 0;
        }

        struct table_list *temp = table_list->next;

        if (table_list->next->next == NULL) {
            table_list->next = NULL;
        } else {
            table_list->next = table_list->next->next;
        }

        clear_table_node(temp);
    } else {
        if (table_list->next == NULL) {
            clear_table_node(table_list);
        } else {
            *table_list_p = table_list->next;
            clear_table_node(table_list);
        }
    }

    return 1;
}


struct table_list *table_list_get(struct table_list *table_list, int32_t index) {
    if (table_list == NULL) {
        return NULL;
    }

    if (index < 0) {
        return NULL;
    }

    for (size_t i = 0; i < index; i++) {
        if (table_list->next != NULL) {
            table_list = table_list->next;
        } else {
            return NULL;
        }
    }

    return table_list;
}


void table_list_clear(struct table_list *table_list) {
    if (table_list == NULL) {
        return;
    }

    if (table_list->value == NULL) {
        my_free(table_list, sizeof(struct table_list));
        return;
    }

    while (table_list->next != NULL) {
        struct table_list *temp = table_list;
        table_list = table_list->next;
        clear_table_node(temp);
    }

    clear_table_node(table_list);
}


int get_table_index_by_name(struct table_list *table_list, const char *name) {
    if (table_list == NULL || table_list->value == NULL) {
        return -1;
    }



    if (strcmp(table_list->value->name, name) == 0) {
        return 0;
    }

    int i = 1;

    while (table_list->next != NULL) {
        table_list = table_list->next;
        if (strcmp(table_list->value->name, name) == 0) {
            return i;
        }
        i++;
    }

    return -1;
}

