#pragma once

#include "launch_db.h"


size_t str_len(const char *str) {
    size_t size = 0;
    while (str[size] != '\0') {
        size++;
    }

    return size;
}


char *scanf_table_name() {
    printf("Enter table name (%d or less symbols): ", TABLE_NAME_SIZE);
    char *table_name = my_malloc(TABLE_NAME_SIZE * sizeof(char));
    scanf("%s", table_name);
    return table_name;
}


uint16_t scanf_num_of_filters() {
    printf("Enter num of filters: ");
    uint16_t num_of_filters;
    scanf("%"SCNu16"", &num_of_filters);
    return num_of_filters;
}


void print_available_filters() {
    printf("Available filter_type:\n");
    printf("0.BOOL_EQ 1.BOOL_NEQ 2.INT_EQ 3.INT_NEQ 4.FLOAT_EQ 5.FLOAT_NEQ 6.STR_EQ 7.STR_NEQ 8.INT_GR 9.INT_LESS \n");
    printf("10.INT_GRE_EQ 11.INT_LESS_EQ 12.FLOAT_GR 13.FLOAT_LESS 14.FLOAT_GRE_EQ 15.FLOAT_LESS_EQ 16.STR_LIKE\n");
}


void *scanf_filter_value(enum filter_type filter_type) {
    switch (filter_type) {
        case BOOL_EQ:
        case BOOL_NEQ: {
            int32_t *value = my_malloc(sizeof(int32_t));
            scanf("%"SCNd32"", value);
            return value;
        }
        case INT_EQ:
        case INT_NEQ:
        case INT_GR:
        case INT_LESS:
        case INT_GRE_EQ:
        case INT_LESS_EQ: {
            int32_t *value = my_malloc(sizeof(int32_t));
            scanf("%"SCNd32"", value);
            return value;
        }
        case FLOAT_EQ:
        case FLOAT_NEQ:
        case FLOAT_GR:
        case FLOAT_LESS:
        case FLOAT_GRE_EQ:
        case FLOAT_LESS_EQ: {
            float *value = my_malloc(sizeof(float));
            scanf("%f", value);
            return value;
        }
        case STR_EQ:
        case STR_NEQ:
        case STR_LIKE: {
            char *s = my_malloc(MAX_STR_SIZE * sizeof(char));
            scanf("%s", s);
            uint16_t size = str_len(s);
            char *value = my_malloc(size * sizeof(char));
            memcpy(value, s, size);
            my_free(s, MAX_STR_SIZE * sizeof(char));
            return value;
        }
    }

    return NULL;
}


struct filter **scanf_filters(uint32_t num_of_filters) {
    struct filter **filters_list = (struct filter **) my_malloc(num_of_filters * sizeof(struct filter *));

    for (size_t i = 0; i < num_of_filters; i++) {
        print_available_filters();
        printf("Enter filter_type num: ");
        enum filter_type filter_type;
        scanf("%d", &filter_type);
        printf("Enter attribute number to which filter is applied: ");
        uint16_t attribute_num;
        scanf("%"SCNu16"", &attribute_num);
        printf("Enter filter attribute value: ");
        void *value = scanf_filter_value(filter_type);

        struct filter *filter = my_malloc(sizeof(struct filter));

        filter->attribute_num = attribute_num;
        filter->filter_type = filter_type;
        filter->value = value;

        filters_list[i] = filter;
    }

    return filters_list;
}


void free_filter_value(void *value, enum filter_type filter_type) {
    switch (filter_type) {
        case BOOL_EQ:
        case BOOL_NEQ:
            my_free(value, sizeof(struct bool_field));
            break;
        case INT_EQ:
        case INT_NEQ:
        case INT_GR:
        case INT_LESS:
        case INT_GRE_EQ:
        case INT_LESS_EQ:
            my_free(value, sizeof(struct int_field));
            break;
        case FLOAT_EQ:
        case FLOAT_NEQ:
        case FLOAT_GR:
        case FLOAT_LESS:
        case FLOAT_GRE_EQ:
        case FLOAT_LESS_EQ:
            my_free(value, sizeof(struct float_field));
            break;
        case STR_EQ:
        case STR_NEQ:
        case STR_LIKE:
            my_free(((struct string_field *) value)->data, ((struct string_field *) value)->size * sizeof(char));
            my_free(value, sizeof(struct string_field));
    }
}


void free_filters(struct filter **filters, uint16_t num_of_filters) {
    for (size_t i = 0; i < num_of_filters; i++) {
        free_filter_value(filters[i]->value, filters[i]->filter_type);
        my_free(filters[i], sizeof(struct filter));
    }

    my_free(filters, num_of_filters * sizeof(struct filter *));
}


struct tuple *scanf_tuple(struct table *table) {
    void **data = my_malloc(table->num_of_columns * sizeof(void *));

    for (size_t i = 0; i < table->num_of_columns; i++) {
        switch (table->table_scheme[i]) {
            case BOOL: {
                data[i] = my_malloc(sizeof(struct bool_field));
                printf("Enter bool attribute %zu: ", i);
                scanf("%"SCNd32"", &((struct bool_field *) data[i])->data);
                break;
            }
            case INT: {
                data[i] = my_malloc(sizeof(struct int_field));
                printf("Enter int attribute %zu: ", i);
                scanf("%"SCNd32"", &((struct int_field *) data[i])->data);
                break;
            }
            case FLOAT: {
                data[i] = my_malloc(sizeof(struct float_field));
                printf("Enter float attribute %zu: ", i);
                scanf("%f", &((struct float_field *) data[i])->data);
                break;
            }
            case STRING: {
                printf("Enter string attribute %zu (%d or less symbols): ", i, MAX_STR_SIZE);
                char *s = my_malloc(MAX_STR_SIZE * sizeof(char));
                scanf("%s", s);
                uint16_t size = str_len(s);
                data[i] = my_malloc(sizeof(struct string_field));
                ((struct string_field *) data[i])->size = size;
                ((struct string_field *) data[i])->data = my_malloc(size * sizeof(char));
                memcpy(((struct string_field *) data[i])->data, s, size);
                my_free(s, MAX_STR_SIZE);
                break;
            }
        }
    }

    return create_tuple((page_offset) {.offset = 0}, data, table->num_of_columns, table->table_scheme);
}


struct column_list *scanf_column_list(uint64_t num_of_columns) {
    struct column_list *column_list = create_column_list();

    for (size_t i = 0; i < num_of_columns; i++) {
        printf("Enter column %zu data type: ", i);
        enum data_type data_type;
        scanf("%d", &data_type);

        printf("Enter column %zu name (%d or less symbols): ", i, COLUMN_NAME_SIZE);
        char *column_name = my_malloc(COLUMN_NAME_SIZE * sizeof(char));
        scanf("%s", column_name);

        uint64_t column_name_size = str_len(column_name);

        char *new_column_name = my_malloc(column_name_size * sizeof(char));
        memcpy(new_column_name, column_name, column_name_size);
        my_free(column_name, COLUMN_NAME_SIZE * sizeof(char));

        struct column *column = create_column(data_type, column_name_size, new_column_name);

        column_list_push(column_list, column);
    }

    return column_list;
}


struct table *scanf_table() {
    printf("Enter table name (%d or less symbols): ", TABLE_NAME_SIZE);
    char *table_name = my_malloc(TABLE_NAME_SIZE * sizeof(char));
    scanf("%s", table_name);
    uint64_t name_size = str_len(table_name);
    char *new_table_name = my_malloc(name_size * sizeof(char));
    memcpy(new_table_name, table_name, name_size);
    my_free(table_name, TABLE_NAME_SIZE * sizeof(char));

    printf("Enter num of columns: ");
    uint64_t num_of_columns;
    scanf("%"SCNu64"", &num_of_columns);

    struct column_list *column_list = scanf_column_list(num_of_columns);

    return create_table(name_size, new_table_name, num_of_columns, column_list);
}


void launch_db() {
    printf("Enter db name (%d or less symbols): ", DB_NAME_SIZE);
    char *db_name = my_malloc(20 * sizeof(char));
    scanf("%s", db_name);

    struct db *db = open_db(db_name);

    enum command_type command = 0;

    while (command != EXIT) {
        printf("Available:\n0. create table\n1. delete table\n2. insert\n3. select\n4. delete\n5. update\n6. join\n7. exit\n");
        printf("Enter command number: ");
        scanf("%d", &command);

        switch (command) {
            case CREATE_TABLE: {
                struct table *table = scanf_table();
                add_table(db, table);
                break;
            }
            case DELETE_TABLE: {
                char *table_name = scanf_table_name();

                delete_table(db, table_name);

                my_free(table_name, TABLE_NAME_SIZE);

                break;
            }
            case INSERT: {
                char *table_name = scanf_table_name();

                struct table_list *list = table_list_get(db->table_list,
                                                         get_table_index_by_name(db->table_list, table_name));

                if (list == NULL) {
                    my_free(table_name, TABLE_NAME_SIZE);
                    break;
                }

                struct table *table = list->value;

                if (table == NULL) {
                    my_free(table_name, TABLE_NAME_SIZE);
                    break;
                }

                struct tuple *tuple = scanf_tuple(table);
                insert_to_table(db, table_name, tuple);
                my_free(table_name, TABLE_NAME_SIZE);
                break;
            }
            case SELECT: {
                char *table_name = scanf_table_name();
                uint16_t num_of_filters = scanf_num_of_filters();
                struct filter **filters = scanf_filters(num_of_filters);

                select_from_table(db, table_name, num_of_filters, filters);

                free_filters(filters, num_of_filters);

                my_free(table_name, TABLE_NAME_SIZE);

                break;
            }
            case DELETE: {
                char *table_name = scanf_table_name();
                uint16_t num_of_filters = scanf_num_of_filters();
                struct filter **filters = scanf_filters(num_of_filters);

                delete_from_table(db, table_name, num_of_filters, filters);

                free_filters(filters, num_of_filters);

                my_free(table_name, TABLE_NAME_SIZE);

                break;
            }
            case UPDATE: {
                // TODO
                break;
            }
            case JOIN : {
                char *table1_name = scanf_table_name();
                char *table2_name = scanf_table_name();

                uint16_t num_of_filters1 = scanf_num_of_filters();
                struct filter **filters1 = scanf_filters(num_of_filters1);

                uint16_t num_of_filters2 = scanf_num_of_filters();
                struct filter **filters2 = scanf_filters(num_of_filters2);

                join_table(db, table1_name, table2_name, num_of_filters1, filters1, num_of_filters2, filters2);

                free_filters(filters2, num_of_filters2);
                free_filters(filters1, num_of_filters1);

                my_free(table2_name, TABLE_NAME_SIZE);
                my_free(table1_name, TABLE_NAME_SIZE);

                break;
            }
            case EXIT:
                close_db(db);
                break;
            default:
                printf("Wrong command\n");
        }
    }
}