#pragma once

#include "launch_db.h"

static char *open_db_string = "open db";
static char *create_db_string = "create db";
static char *close_db_string = "close db";
static char *create_table_string = "create table";
static char *delete_table_string = "delete table";
static char *insert_string = "insert";
static char *select_string = "select";
static char *delete_string = "delete";
static char *update_string = "update";
static char *join_string = "join";
static char *exit_string = "exit";

static void print_available_commands() {
    printf("%d: %s\n", OPEN_DB, open_db_string);
    printf("%d: %s\n", CREATE_DB, create_db_string);
    printf("%d: %s\n", CLOSE_DB, close_db_string);
    printf("%d: %s\n", CREATE_TABLE, create_table_string);
    printf("%d: %s\n", DELETE_TABLE, delete_table_string);
    printf("%d: %s\n", INSERT, insert_string);
    printf("%d: %s\n", SELECT, select_string);
    printf("%d: %s\n", DELETE, delete_string);
    printf("%d: %s\n", UPDATE, update_string);
    printf("%d: %s\n", JOIN, join_string);
    printf("%d: %s\n", EXIT, exit_string);
}

static struct update_value *scanf_update_value(const enum data_type *table_scheme) {
    struct update_value *update_value = my_malloc(sizeof(struct update_value));
    printf("Enter updated attribute num: ");
    uint16_t attribute_num;
    scanf("%"SCNu16"", &attribute_num);
    update_value->attribute_num = attribute_num;

    switch (table_scheme[attribute_num]) {
        case BOOL: {
            printf("Enter updated bool attribute value: ");
            update_value->value = my_malloc(sizeof(int32_t));
            scanf("%"SCNd32"", (int32_t *) update_value->value);
            break;
        }
        case INT: {
            printf("Enter updated int attribute value: ");
            update_value->value = my_malloc(sizeof(int32_t));
            scanf("%"SCNd32"", (int32_t *) update_value->value);
            break;
        }
        case FLOAT: {
            printf("Enter updated float attribute value: ");
            update_value->value = my_malloc(sizeof(float));
            scanf("%f", (float *) update_value->value);
            break;
        }
        case STRING: {
            printf("Enter updated string attribute value: ");
            char s[100];
            scanf("%s", s);
            uint16_t size = strlen(s);
            update_value->value = my_malloc(size * sizeof(char));
            memcpy(update_value->value, s, size);
            break;
        }
    }

    return update_value;
}

static struct update_query *scanf_update_query(enum data_type *table_scheme) {
    struct update_query *update_query = my_malloc(sizeof(struct update_query));
    printf("Enter num of updated values: ");
    uint16_t num_of_updates;
    scanf("%"SCNu16"", &num_of_updates);

    update_query->num_of_updates = num_of_updates;
    update_query->update_values = my_malloc(num_of_updates * sizeof(struct update_value *));

    for (size_t i = 0; i < num_of_updates; i++) {
        update_query->update_values[i] = scanf_update_value(table_scheme);
    }

    return update_query;
}

static void free_update_value(struct update_value *update_value, const enum data_type *table_scheme) {
    switch (table_scheme[update_value->attribute_num]) {
        case BOOL: {
            my_free(update_value->value, sizeof(int32_t));
            break;
        }
        case INT: {
            my_free(update_value->value, sizeof(int32_t));
            break;
        }
        case FLOAT: {
            my_free(update_value->value, sizeof(float));
            break;
        }
        case STRING: {
            uint16_t size = strlen(update_value->value);
            my_free(update_value->value, size * sizeof(char));
            break;
        }
    }

    my_free(update_value, sizeof(struct update_value));
}

static void free_update_query(struct update_query *update_query, enum data_type *table_scheme) {
    for (size_t i = 0; i < update_query->num_of_updates; i++) {
        free_update_value(update_query->update_values[i], table_scheme);
    }

    my_free(update_query->update_values, update_query->num_of_updates * sizeof(struct update_value *));
    my_free(update_query, sizeof(struct update_query));
}

static size_t str_len(const char *str) {
    size_t size = 0;
    while (str[size] != '\0') {
        size++;
    }

    return size;
}

static char *scanf_table_name() {
    printf("Enter table name (%d or less symbols): ", TABLE_NAME_SIZE);
    char *table_name = my_malloc(TABLE_NAME_SIZE * sizeof(char));
    scanf("%s", table_name);
    return table_name;
}

static uint16_t scanf_num_of_filters() {
    printf("Enter num of filters: ");
    uint16_t num_of_filters;
    scanf("%"SCNu16"", &num_of_filters);
    return num_of_filters;
}

static void print_available_filters() {
    printf("Available filter_cond:\n");
    printf("0.BOOL_EQ 1.BOOL_NEQ 2.INT_EQ 3.INT_NEQ 4.FLOAT_EQ 5.FLOAT_NEQ 6.STR_EQ 7.STR_NEQ 8.INT_GR 9.INT_LESS \n");
    printf("10.INT_GR_EQ 11.INT_LESS_EQ 12.FLOAT_GR 13.FLOAT_LESS 14.FLOAT_GR_EQ 15.FLOAT_LESS_EQ 16.STR_LIKE\n");
}

static void *scanf_filter_value(enum filter_cond filter_cond, enum filter_type filter_type) {
    if (filter_type == REFERENCE) {
        char s[100];
        scanf("%s", s);
        char *value = my_malloc(strlen(s) * sizeof(char));
        memcpy(value, s, strlen(s));
        return value;
    }

    switch (filter_cond) {
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
        case INT_GR_EQ:
        case INT_LESS_EQ: {
            int32_t *value = my_malloc(sizeof(int32_t));
            scanf("%"SCNd32"", value);
            return value;
        }
        case FLOAT_EQ:
        case FLOAT_NEQ:
        case FLOAT_GR:
        case FLOAT_LESS:
        case FLOAT_GR_EQ:
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

static struct filter **scanf_filters(uint32_t num_of_filters) {
    struct filter **filters_list = (struct filter **) my_malloc(num_of_filters * sizeof(struct filter *));

    for (size_t i = 0; i < num_of_filters; i++) {
        printf("Enter col name to which filter is applied: ");
        char s[100];
        scanf("%s", s);
        char *col_name = my_malloc(strlen(s) * sizeof(char));
        memcpy(col_name, s, strlen(s));

        printf("Enter filter type (0 - CONST, 1 - REFERENCE): ");
        enum filter_type filter_type;
        scanf("%d", &filter_type);

        print_available_filters();
        printf("Enter filter_cond num: ");
        enum filter_cond filter_cond;
        scanf("%d", &filter_cond);

        printf("Enter filter attribute value: ");
        void *value = scanf_filter_value(filter_cond, filter_type);

        struct filter *filter = my_malloc(sizeof(struct filter));

        filter->col_name = col_name;
        filter->filter_type = filter_type;
        filter->filter_cond = filter_cond;
        filter->value = value;

        filters_list[i] = filter;
    }

    return filters_list;
}

static struct tuple *scanf_tuple(struct table *table) {
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
                char s[MAX_STR_SIZE];
                scanf("%s", s);

                struct string_field *string_field = my_malloc(sizeof(struct string_field));
                uint16_t size = str_len(s);

                string_field->size = size;
                string_field->data = my_malloc(size * sizeof(char));
                memcpy(string_field->data, s, size);

                data[i] = string_field;

                break;
            }
        }
    }

    return create_tuple(data, table->num_of_columns, table->table_scheme);
}

static struct column **scanf_columns(uint64_t num_of_columns) {
    struct column **columns = my_malloc(num_of_columns * sizeof(struct column *));

    for (size_t i = 0; i < num_of_columns; i++) {
        printf("Enter column %zu data type: ", i);
        enum data_type data_type;
        scanf("%d", &data_type);

        printf("Enter column %zu name (%d or less symbols): ", i, COLUMN_NAME_SIZE);
        char column_name[COLUMN_NAME_SIZE];
        scanf("%s", column_name);

        uint64_t column_name_size = str_len(column_name);

        char *new_column_name = my_malloc(column_name_size * sizeof(char));
        memcpy(new_column_name, column_name, column_name_size);

        struct column *column = create_column(data_type, column_name_size, new_column_name);

        columns[i] = column;
    }

    return columns;
}

static struct table *scanf_table() {
    printf("Enter table name (%d or less symbols): ", TABLE_NAME_SIZE);
    char table_name[TABLE_NAME_SIZE];
    scanf("%s", table_name);
    uint64_t name_size = str_len(table_name);
    char *new_table_name = my_malloc(name_size * sizeof(char));
    memcpy(new_table_name, table_name, name_size);

    printf("Enter num of columns: ");
    uint64_t num_of_columns;
    scanf("%"SCNu64"", &num_of_columns);

    struct column **columns = scanf_columns(num_of_columns);

    struct table *table = my_malloc(sizeof(struct table));
    table->name_size = name_size;
    table->name = new_table_name;
    table->num_of_columns = num_of_columns;
    table->columns = columns;

    enum data_type *table_scheme = my_malloc(num_of_columns * sizeof(enum data_type));

    for (size_t i = 0; i < num_of_columns; i++) {
        table_scheme[i] = columns[i]->data_type;
    }

    table->table_scheme = table_scheme;

    return table;
}


static struct join_values *scanf_join_value() {
    struct join_values *join_values = my_malloc(sizeof(struct join_values));

    printf("Enter num of join values: ");
    uint16_t num_of_join_values;
    scanf("%"SCNu16"", &num_of_join_values);

    join_values->num_of_join_values = num_of_join_values;
    join_values->table1_column_names = my_malloc(num_of_join_values * sizeof(char *));
    join_values->table2_column_names = my_malloc(num_of_join_values * sizeof(char *));
    join_values->filter_cond = my_malloc(num_of_join_values * sizeof(enum filter_cond));

    for (size_t i = 0; i < num_of_join_values; i++) {
        printf("Enter first table join column name %zu: ", i);
        char table1_col_name[100];
        scanf("%s", table1_col_name);
        join_values->table1_column_names[i] = my_malloc(strlen(table1_col_name) * sizeof(char));
        memcpy(join_values->table1_column_names[i], table1_col_name, strlen(table1_col_name));

        printf("Enter second table join column name %zu: ", i);
        char table2_col_name[100];
        scanf("%s", table2_col_name);
        join_values->table2_column_names[i] = my_malloc(strlen(table2_col_name) * sizeof(char));
        memcpy(join_values->table2_column_names[i], table2_col_name, strlen(table2_col_name));

        print_available_filters();
        printf("Enter filter %zu condition ", i);
        enum filter_cond filter_cond;
        scanf("%d", &filter_cond);
        join_values->filter_cond[i] = filter_cond;
    }

    return join_values;
}


static void free_join_values(struct join_values *join_values) {
    for (size_t i = 0; i < join_values->num_of_join_values; i++) {
        my_free(join_values->table2_column_names[i], strlen(join_values->table2_column_names[i]) * sizeof(char));
        my_free(join_values->table1_column_names[i], strlen(join_values->table1_column_names[i]) * sizeof(char));
    }

    my_free(join_values->filter_cond, join_values->num_of_join_values * sizeof(enum filter_cond));
    my_free(join_values->table2_column_names, join_values->num_of_join_values * sizeof(char *));
    my_free(join_values->table1_column_names, join_values->num_of_join_values * sizeof(char *));
    my_free(join_values, sizeof(struct join_values));
}


void launch_db() {
    printf("Enter db name (%d or less symbols): ", DB_NAME_SIZE);
    char *db_name = my_malloc(20 * sizeof(char));
    scanf("%s", db_name);

    struct db *db = open_db(db_name);

    enum command_type command = 0;

    while (command != EXIT) {
        print_available_commands();
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

                struct table *table = get_table_by_name(table_name, db->num_of_tables, db->tables);

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

                free_filters(num_of_filters, filters);

                my_free(table_name, TABLE_NAME_SIZE);

                break;
            }
            case DELETE: {
                char *table_name = scanf_table_name();
                uint16_t num_of_filters = scanf_num_of_filters();
                struct filter **filters = scanf_filters(num_of_filters);

                delete_from_table(db, table_name, num_of_filters, filters);

                free_filters(num_of_filters, filters);

                my_free(table_name, TABLE_NAME_SIZE);

                break;
            }
            case UPDATE: {
//                char *table_name = scanf_table_name();
//                uint16_t num_of_filters = scanf_num_of_filters();
//                struct filter **filters = scanf_filters(num_of_filters);
//
//                struct table_list *table_list = table_list_get(
//                        db->table_list, get_table_index_by_name(db->table_list, table_name));
//
//                if (table_list == NULL) {
//                    break;
//                }
//
//                struct table *table = table_list->value;
//
//                if (table == NULL) {
//                    break;
//                }
//
//                struct update_query *update_query = scanf_update_query(table->table_scheme);
//
//                update_table(db, table_name, num_of_filters, filters, update_query);
//
//                free_update_query(update_query, table->table_scheme);
//                free_filters(filters, num_of_filters);
//                my_free(table_name, TABLE_NAME_SIZE);
                break;
            }
            case JOIN : {
                char *table1_name = scanf_table_name();
                char *table2_name = scanf_table_name();

                uint16_t num_of_filters1 = scanf_num_of_filters();
                struct filter **filters1 = scanf_filters(num_of_filters1);

                uint16_t num_of_filters2 = scanf_num_of_filters();
                struct filter **filters2 = scanf_filters(num_of_filters2);


                struct join_values *join_values = scanf_join_value();

                join_table(db, table1_name, table2_name, num_of_filters1, filters1,
                           num_of_filters2, filters2, join_values);

                free_join_values(join_values);

                free_filters(num_of_filters2, filters2);
                free_filters(num_of_filters1, filters1);

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