#pragma once

#include "util.h"

// launch_db
struct update_value *scanf_update_value(const enum data_type *table_scheme) {
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

struct update_query *scanf_update_query(enum data_type *table_scheme) {
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

void free_update_value(struct update_value *update_value, const enum data_type *table_scheme) {
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

void free_update_query(struct update_query *update_query, enum data_type *table_scheme) {
    for (size_t i = 0; i < update_query->num_of_updates; i++) {
        free_update_value(update_query->update_values[i], table_scheme);
    }

    my_free(update_query->update_values, update_query->num_of_updates * sizeof(struct update_value *));
    my_free(update_query, sizeof(struct update_query));
}

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
    printf("10.INT_GR_EQ 11.INT_LESS_EQ 12.FLOAT_GR 13.FLOAT_LESS 14.FLOAT_GR_EQ 15.FLOAT_LESS_EQ 16.STR_LIKE\n");
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
            my_free(value, sizeof(int32_t));
            break;
        case INT_EQ:
        case INT_NEQ:
        case INT_GR:
        case INT_LESS:
        case INT_GR_EQ:
        case INT_LESS_EQ:
            my_free(value, sizeof(int32_t));
            break;
        case FLOAT_EQ:
        case FLOAT_NEQ:
        case FLOAT_GR:
        case FLOAT_LESS:
        case FLOAT_GR_EQ:
        case FLOAT_LESS_EQ:
            my_free(value, sizeof(float));
            break;
        case STR_EQ:
        case STR_NEQ:
        case STR_LIKE: {
            size_t size = strlen(value);
            my_free(value, size);
            break;
        }
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

// tests util
struct table *create_test_table1() {
    struct column_list *column_list1 = create_column_list();
    struct column *col1_1 = my_malloc(sizeof(struct column));
    col1_1->data_type = INT;
    col1_1->name_size = 16;
    col1_1->name = my_malloc(col1_1->name_size * sizeof(char));
    memcpy(col1_1->name, "I LIKE THIS NAME", col1_1->name_size);
    struct column *col1_2 = my_malloc(sizeof(struct column));
    col1_2->data_type = FLOAT;
    col1_2->name_size = 10;
    col1_2->name = my_malloc(col1_2->name_size * sizeof(char));
    memcpy(col1_2->name, "name of 10", col1_2->name_size);
    struct column *col1_3 = my_malloc(sizeof(struct column));
    col1_3->data_type = STRING;
    col1_3->name_size = 9;
    col1_3->name = my_malloc(col1_3->name_size * sizeof(char));
    memcpy(col1_3->name, "col1_3 name", col1_3->name_size);
    column_list_push(column_list1, col1_1);
    column_list_push(column_list1, col1_2);
    column_list_push(column_list1, col1_3);

    file_offset table1_offset;
    table1_offset.offset = PAGE_SIZE;
    uint64_t table1_name_size = 11;
    char *table1_name = my_malloc(table1_name_size);
    memcpy(table1_name, "table1", table1_name_size);
    uint64_t table1_num_of_columns = 3;
    struct table *table1 = create_table(table1_name_size, table1_name, table1_num_of_columns, column_list1);

    return table1;
}

struct table *create_test_table2() {
    struct column_list *column_list2 = create_column_list();
    struct column *col2_1 = my_malloc(sizeof(struct column));
    col2_1->data_type = FLOAT;
    col2_1->name_size = 12;
    col2_1->name = my_malloc(col2_1->name_size * sizeof(char));
    memcpy(col2_1->name, "float_column", col2_1->name_size);
    struct column *col2_2 = my_malloc(sizeof(struct column));
    col2_2->data_type = INT;
    col2_2->name_size = 10;
    col2_2->name = my_malloc(col2_2->name_size * sizeof(char));
    memcpy(col2_2->name, "int_column", col2_2->name_size);
    struct column *col2_3 = my_malloc(sizeof(struct column));
    col2_3->data_type = STRING;
    col2_3->name_size = 14;
    col2_3->name = my_malloc(col2_3->name_size * sizeof(char));
    memcpy(col2_3->name, "string_column1", col2_3->name_size);
    struct column *col2_4 = my_malloc(sizeof(struct column));
    col2_4->data_type = STRING;
    col2_4->name_size = 14;
    col2_4->name = my_malloc(col2_4->name_size * sizeof(char));
    memcpy(col2_4->name, "string_column2", col2_4->name_size);
    column_list_push(column_list2, col2_1);
    column_list_push(column_list2, col2_2);
    column_list_push(column_list2, col2_3);
    column_list_push(column_list2, col2_4);

    file_offset table2_offset;
    table2_offset.offset = 10 * PAGE_SIZE;
    uint64_t table2_name_size = 6;
    char *table2_name = my_malloc(table2_name_size);
    memcpy(table2_name, "table2", table2_name_size);
    uint64_t table2_num_of_columns = 4;
    struct table *table2 = create_table(table2_name_size, table2_name, table2_num_of_columns, column_list2);

    return table2;
}

struct table *create_test_table3() {
    struct column_list *column_list3 = create_column_list();
    struct column *col3_1 = my_malloc(sizeof(struct column));
    col3_1->data_type = STRING;
    col3_1->name_size = 6;
    col3_1->name = my_malloc(col3_1->name_size * sizeof(char));
    memcpy(col3_1->name, "some_name", col3_1->name_size);
    struct column *col3_2 = my_malloc(sizeof(struct column));
    col3_2->data_type = STRING;
    col3_2->name_size = 4;
    col3_2->name = my_malloc(col3_2->name_size * sizeof(char));
    memcpy(col3_2->name, "col3_2", col3_2->name_size);
    struct column *col3_3 = my_malloc(sizeof(struct column));
    col3_3->data_type = STRING;
    col3_3->name_size = 9;
    col3_3->name = my_malloc(col3_3->name_size * sizeof(char));
    memcpy(col3_3->name, "name", col3_3->name_size);
    struct column *col3_4 = my_malloc(sizeof(struct column));
    col3_4->data_type = STRING;
    col3_4->name_size = 11;
    col3_4->name = my_malloc(col3_4->name_size * sizeof(char));
    memcpy(col3_4->name, "hippopotamus", col3_4->name_size);
    column_list_push(column_list3, col3_1);
    column_list_push(column_list3, col3_2);
    column_list_push(column_list3, col3_3);
    column_list_push(column_list3, col3_4);

    file_offset table3_offset;
    table3_offset.offset = 3 * PAGE_SIZE;
    uint64_t table3_name_size = 10;
    char *table3_name = my_malloc(table3_name_size);
    memcpy(table3_name, "last_table", table3_name_size);
    uint64_t table3_num_of_columns = 4;
    struct table *table3 = create_table(table3_name_size, table3_name, table3_num_of_columns, column_list3);

    return table3;
}

void show_table(struct table *table) {
    printf("\tlast page offset: %"PRIu64"\n", table->last_page_offset.offset);
    printf("\tname size: %"PRIu64"\n", table->name_size);
    printf("\tname: %s\n", table->name);
    printf("\tnum of columns: %"PRIu64"\n", table->num_of_columns);

    for (size_t j = 0; j < table->num_of_columns; j++) {
        struct column *column = column_list_get(table->column_list, j)->value;
        printf("\tcolumn %zu\n", j);
        printf("\t\tdata type: %d\n", column->data_type);
        printf("\t\tname size: %"PRIu64"\n", column->name_size);
        printf("\t\tname: %s\n", column->name);
    }

    printf("\n\n");
}

void show_table_list(struct table_list *table_list) {
    int i = 1;
    while (table_list->next != NULL) {
        printf("table %d", i++);
        show_table(table_list->value);
        table_list = table_list->next;
    }

    printf("table %d", i++);
    show_table(table_list->value);
}

void print_page(struct page *page, uint32_t num_of_attributes, enum data_type *table_scheme) {
    printf("page offset: %"PRIu64"\n", page->offset.offset);
    printf("prev page offset: %"PRIu64"\n", page->prev_page.offset);
    printf("next page offset: %"PRIu64"\n", page->next_page.offset);
    printf("free space offset: %"PRIu16"\n", page->free_space_offset.offset);
    printf("num of tuples: %"PRIu64"\n", page->num_of_tuples);
    printf("total free space size: %"PRIu16"\n", page->total_free_space_size);
    for (size_t i = 0; i < page->num_of_tuples; i++) {
        printf("tuple %zu\n", i);
        print_tuple(page->tuples[i], num_of_attributes, table_scheme);
    }
}

char *create_random_string(int32_t size) {
    char *s = my_malloc(size);
    for (size_t i = 0; i < size; i++) {
        char random_char = (char) ('A' + rand() % 26);
        memcpy(&s[i], &random_char, 1);
    }

    return s;
}

float create_random_float() {
    return (float) rand() / (float) (RAND_MAX / FLOAT_RANGE);
}

struct tuple *create_random_tuple(uint64_t num_of_attributes, const enum data_type *table_scheme) {
    void **data = my_malloc(num_of_attributes * sizeof(void *));

    for (size_t i = 0; i < num_of_attributes; i++) {
        switch (table_scheme[i]) {
            case BOOL: {
                struct bool_field *bool_field = my_malloc(sizeof(struct bool_field));
                bool_field->data = rand() % 2;
                data[i] = bool_field;
                break;
            }
            case INT: {
                struct int_field *int_field = my_malloc(sizeof(struct int_field));
                int_field->data = rand() % INT_RANGE;
                data[i] = int_field;
                break;
            }
            case FLOAT : {
                struct float_field *float_field = my_malloc(sizeof(struct float_field));
                float_field->data = create_random_float();
                data[i] = float_field;
                break;
            }
            case STRING: {
                struct string_field *string_field = my_malloc(sizeof(struct string_field));
                string_field->size = (rand() % RANDOM_STRING_SIZE) + 1;
                string_field->data = create_random_string(string_field->size);
                data[i] = string_field;
                break;
            }
        }
    }

    return create_tuple((page_offset) {.offset = 0}, data, num_of_attributes, table_scheme);
}

struct filter *create_random_filter(uint64_t num_of_attributes, const enum data_type *table_scheme) {
    struct filter *filter = my_malloc(sizeof(struct filter));

    filter->attribute_num = rand() % num_of_attributes;

    switch (table_scheme[filter->attribute_num]) {
        case BOOL: {
            filter->filter_type = rand() % 2;
            int32_t *value = my_malloc(sizeof(int32_t));
            *value = rand() % 2;
            filter->value = value;
            break;
        }
        case INT: {
            int32_t filter_type = rand() % 6;
            if (filter_type == 0) {
                filter->filter_type = INT_EQ;
            } else if (filter_type == 1) {
                filter->filter_type = INT_NEQ;
            } else if (filter_type == 2) {
                filter->filter_type = INT_GR;
            } else if (filter_type == 3) {
                filter->filter_type = INT_LESS;
            } else if (filter_type == 4) {
                filter->filter_type = INT_GR_EQ;
            } else if (filter_type == 5) {
                filter->filter_type = INT_LESS_EQ;
            }
            int32_t *value = my_malloc(sizeof(int32_t));
            *value = rand() % INT_RANGE;
            filter->value = value;
            break;
        }
        case FLOAT: {
            int32_t filter_type = rand() % 6;
            if (filter_type == 0) {
                filter->filter_type = FLOAT_EQ;
            } else if (filter_type == 1) {
                filter->filter_type = FLOAT_NEQ;
            } else if (filter_type == 2) {
                filter->filter_type = FLOAT_GR;
            } else if (filter_type == 3) {
                filter->filter_type = FLOAT_LESS;
            } else if (filter_type == 4) {
                filter->filter_type = FLOAT_GR_EQ;
            } else if (filter_type == 5) {
                filter->filter_type = FLOAT_LESS_EQ;
            }
            float *value = my_malloc(sizeof(float));
            *value = create_random_float();
            filter->value = value;
            break;
        }
        case STRING: {
            int32_t filter_type = rand() % 3;
            if (filter_type == 0) {
                filter->filter_type = STR_EQ;
                filter->value = create_random_string(7);
            } else if (filter_type == 1) {
                filter->filter_type = STR_NEQ;
                filter->value = create_random_string(7);
            } else if (filter_type == 2) {
                filter->filter_type = STR_LIKE;
                filter->value = create_random_string(2);
            }
            break;
        }
    }

    return filter;
}

struct filter **create_random_filters(uint16_t num_of_filters, uint64_t num_of_attrs, enum data_type *table_scheme) {
    struct filter **filters = my_malloc(num_of_filters * sizeof(struct filter *));

    for (size_t i = 0; i < num_of_filters; i++) {
        filters[i] = create_random_filter(num_of_attrs, table_scheme);
    }

    return filters;
}

void print_db_scheme(struct db *db) {
    printf("db name: %s\n", db->name);
    printf("num of tables: %"PRIu64"\n", db->num_of_tables);
    printf("last page offset: %"PRIu64"\n", db->last_page_offset.offset);
    for (size_t i = 0; i < db->num_of_tables; i++) {
        struct table *table = table_list_get(db->table_list, i)->value;
        printf("table %zu\n", i);
        printf("\tlast page offset: %"PRIu64"\n", table->last_page_offset.offset);
        printf("\tname size: %"PRIu64"\n", table->name_size);
        printf("\tname: %s\n", table->name);
        printf("\tnum of columns: %"PRIu64"\n", table->num_of_columns);

        for (size_t j = 0; j < table->num_of_columns; j++) {
            struct column *column = column_list_get(table->column_list, j)->value;
            printf("\tcolumn %zu\n", j);
            printf("\t\tdata type: %d\n", column->data_type);
            printf("\t\tname size: %"PRIu64"\n", column->name_size);
            printf("\t\tname: %s\n", column->name);
        }
    }
    printf("\n\n");
}

