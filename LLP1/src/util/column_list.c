#pragma once

#include "column_list.h"


void clear_column_node(struct column_list *column_list) {
    free_column(column_list->value);
    my_free(column_list, sizeof(struct column_list));
}


struct column_list *create_column_list() {
    struct column_list *column_list = my_malloc(sizeof(struct column_list));
    column_list->value = NULL;
    column_list->next = NULL;
    return column_list;
}


void column_list_push(struct column_list *column_list, struct column *value) {
    if (column_list->value == NULL) {
        column_list->value = value;
    } else {
        while (column_list->next != NULL) {
            column_list = column_list->next;
        }
        column_list->next = create_column_list();
        column_list->next->value = value;
    }
}


int column_list_remove(struct column_list **column_list_p, size_t index) {
    struct column_list *column_list = *column_list_p;

    if (column_list == NULL || column_list->value == NULL) {
        return 0;
    }

    if (index != 0) {
        for (size_t i = 0; i < index - 1; i++) {
            if (column_list->next != NULL) {
                column_list = column_list->next;
            } else {
                return 0;
            }
        }

        if (column_list->next == NULL) {
            return 0;
        }

        struct column_list *temp = column_list->next;

        if (column_list->next->next == NULL) {
            column_list->next = NULL;
        } else {
            column_list->next = column_list->next->next;
        }

        clear_column_node(temp);
    } else {
        if (column_list->next == NULL) {
            clear_column_node(column_list);
        } else {
            *column_list_p = column_list->next;
            clear_column_node(column_list);
        }
    }

    return 1;
}


struct column_list *column_list_get(struct column_list *column_list, size_t index) {
    for (size_t i = 0; i < index; i++) {
        if (column_list->next != NULL) {
            column_list = column_list->next;
        } else {
            return NULL;
        }
    }

    return column_list;
}


void column_list_clear(struct column_list *column_list) {
    if (column_list == NULL) {
        return;
    }

    if (column_list->value == NULL) {
        my_free(column_list, sizeof(struct column_list));
        return;
    }

    while (column_list->next != NULL) {
        struct column_list *temp = column_list;
        column_list = column_list->next;
        clear_column_node(temp);
    }

    clear_column_node(column_list);
}

