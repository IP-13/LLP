#pragma once

#include "tests.h"

#define INT_RANGE 1000000
#define FLOAT_RANGE 1000
#define RANDOM_STRING_SIZE 20
#define MAX_STR_SIZE 1024
#define TABLE_NAME_SIZE 20
#define COLUMN_NAME_SIZE 20
#define DB_NAME_SIZE 40

// tests util
static struct table *create_test_table1() {
    struct column **columns1 = my_malloc(3 * sizeof(struct column *));

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

    columns1[0] = col1_1;
    columns1[1] = col1_2;
    columns1[2] = col1_3;

    file_offset table1_offset;
    table1_offset.offset = PAGE_SIZE;
    uint64_t table1_name_size = 6;
    char *table1_name = my_malloc(table1_name_size);
    memcpy(table1_name, "table1", table1_name_size);
    uint64_t table1_num_of_columns = 3;
    struct table *table1 = create_table(table1_offset, table1_name_size, table1_name, table1_num_of_columns, columns1);

    return table1;
}

static struct table *create_test_table2() {
    struct column **columns2 = my_malloc(4 * sizeof(struct column *));

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

    columns2[0] = col2_1;
    columns2[1] = col2_2;
    columns2[2] = col2_3;
    columns2[3] = col2_4;

    file_offset table2_offset;
    table2_offset.offset = 10 * PAGE_SIZE;
    uint64_t table2_name_size = 6;
    char *table2_name = my_malloc(table2_name_size);
    memcpy(table2_name, "table2", table2_name_size);
    uint64_t table2_num_of_columns = 4;
    struct table *table2 = create_table(table2_offset, table2_name_size, table2_name, table2_num_of_columns, columns2);

    return table2;
}

static struct table *create_test_table3() {
    struct column **columns3 = my_malloc(4 * sizeof(struct column *));

    struct column *col3_1 = my_malloc(sizeof(struct column));
    col3_1->data_type = BOOL;
    col3_1->name_size = 11;
    col3_1->name = my_malloc(col3_1->name_size * sizeof(char));
    memcpy(col3_1->name, "some_column", col3_1->name_size);

    struct column *col3_2 = my_malloc(sizeof(struct column));
    col3_2->data_type = INT;
    col3_2->name_size = 6;
    col3_2->name = my_malloc(col3_2->name_size * sizeof(char));
    memcpy(col3_2->name, "col3_2", col3_2->name_size);

    struct column *col3_3 = my_malloc(sizeof(struct column));
    col3_3->data_type = FLOAT;
    col3_3->name_size = 11;
    col3_3->name = my_malloc(col3_3->name_size * sizeof(char));
    memcpy(col3_3->name, "column_name", col3_3->name_size);

    struct column *col3_4 = my_malloc(sizeof(struct column));
    col3_4->data_type = STRING;
    col3_4->name_size = 6;
    col3_4->name = my_malloc(col3_4->name_size * sizeof(char));
    memcpy(col3_4->name, "qwerty", col3_4->name_size);

    columns3[0] = col3_1;
    columns3[1] = col3_2;
    columns3[2] = col3_3;
    columns3[3] = col3_4;

    file_offset table3_offset;
    table3_offset.offset = 3 * PAGE_SIZE;
    uint64_t table3_name_size = 10;
    char *table3_name = my_malloc(table3_name_size);
    memcpy(table3_name, "last_table", table3_name_size);
    uint64_t table3_num_of_columns = 4;
    struct table *table3 = create_table(table3_offset, table3_name_size, table3_name, table3_num_of_columns, columns3);

    return table3;
}

static void show_table(struct table *table) {
    printf("\tlast page offset: %"PRIu64"\n", table->last_page_offset.offset);
    printf("\tname size: %"PRIu64"\n", table->name_size);
    printf("\tname: %s\n", table->name);
    printf("\tnum of columns: %"PRIu64"\n", table->num_of_columns);

    for (size_t j = 0; j < table->num_of_columns; j++) {
        struct column *column = table->columns[j];
        printf("\tcolumn %zu\n", j);
        printf("\t\tdata type: %d\n", column->data_type);
        printf("\t\tname size: %"PRIu64"\n", column->name_size);
        printf("\t\tname: %s\n", column->name);
    }

    printf("\n\n");
}


static void print_page(struct page *page, uint32_t num_of_columns, enum data_type *table_scheme) {
    printf("page offset: %"PRIu64"\n", page->offset.offset);
    printf("prev page offset: %"PRIu64"\n", page->prev_page.offset);
    printf("next page offset: %"PRIu64"\n", page->next_page.offset);
    printf("num of tuples: %"PRIu64"\n", page->num_of_tuples);
    printf("total free space size: %"PRIu16"\n", page->total_free_space_size);
    for (size_t i = 0; i < page->num_of_tuples; i++) {
        printf("tuple %zu\n", i);
        print_tuple(page->tuples[i], num_of_columns, table_scheme);
    }
}

static char *create_random_string(int32_t size) {
    char *s = my_malloc(size);
    for (size_t i = 0; i < size; i++) {
        char random_char = (char) ('A' + rand() % 26);
        memcpy(&s[i], &random_char, 1);
    }

    return s;
}

static float create_random_float() {
    return (float) rand() / (float) (RAND_MAX / FLOAT_RANGE);
}

static struct tuple *create_random_tuple(uint64_t num_of_columns, const enum data_type *table_scheme) {
    void **data = my_malloc(num_of_columns * sizeof(void *));

    for (size_t i = 0; i < num_of_columns; i++) {
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

    return create_tuple(data, num_of_columns, table_scheme);
}


static void **read_tuple_data_from_file(enum data_type *table_scheme, uint64_t num_of_columns, FILE *data_file) {
    void **data = my_malloc(sizeof(void *));

    for (size_t i = 0; i < num_of_columns; i++) {
        switch (table_scheme[i]) {
            case BOOL: {
                struct bool_field *bool_field = my_malloc(sizeof(struct bool_field));
                fscanf(data_file, "%"SCNd32"", &bool_field->data);
                data[i] = bool_field;
            }
            case INT: {
                struct int_field *int_field = my_malloc(sizeof(struct int_field));
                fscanf(data_file, "%"SCNd32"", &int_field->data);
                data[i] = int_field;
            }
            case FLOAT: {
                struct float_field *float_field = my_malloc(sizeof(struct float_field));
                fscanf(data_file, "%f", &float_field->data);
                data[i] = float_field;
            }
            case STRING: {
                struct string_field *string_field = my_malloc(sizeof(struct string_field));
                char s[MAX_STR_SIZE];
                fscanf(data_file, "%s", s);
                uint16_t size = strlen(s);
                string_field->size = size;
                string_field->data = my_malloc(size * sizeof(char));
                memcpy(string_field->data, s, size);
                data[i] = string_field;
            }
        }
    }

    return data;
}


void insert_from_file(char *db_filename, char *table_name, char *data_file_name) {
    uint64_t db_name_size = strlen(db_filename);
    char *my_db_filename = my_malloc(db_name_size * sizeof(char));
    memcpy(my_db_filename, db_filename, db_name_size);
    struct db *db = open_db(my_db_filename);

    uint64_t num_of_columns = 2;

    enum data_type col1_data_type = STRING;
    uint64_t col1_name_size = 4;
    char *col1_name = my_malloc(col1_name_size);
    memcpy(col1_name, "col1", col1_name_size);
    struct column *column1 = create_column(col1_data_type, col1_name_size, col1_name);

    enum data_type col2_data_type = FLOAT;
    uint64_t col2_name_size = 4;
    char *col2_name = my_malloc(col1_name_size);
    memcpy(col2_name, "col2", col2_name_size);
    struct column *column2 = create_column(col2_data_type, col2_name_size, col2_name);

    struct column **columns = my_malloc(2 * sizeof(struct column *));
    columns[0] = column1;
    columns[1] = column2;

    uint64_t table_name_size = strlen(table_name);

    char *my_table_name = my_malloc(table_name_size);

    memcpy(my_table_name, table_name, table_name_size);

    struct table *table = create_table((file_offset) {.offset = NULL_PAGE}, table_name_size, my_table_name,
                                       num_of_columns, columns);

    add_table(db, table);

    FILE *data_file = fopen(data_file_name, "a+");

    for (size_t i = 0; i < 1; i++) {
        void **data = read_tuple_data_from_file(table->table_scheme, num_of_columns, data_file);
        struct tuple *tuple = create_tuple(data, num_of_columns, table->table_scheme);
        insert_to_table(db, my_table_name, tuple);
    }

    fclose(data_file);

    close_db(db);
}

// module tests
void open_db_test() {
    struct db *db = open_db("my_file.txt");

    FILE *file = db->file;

    struct table *table1 = create_test_table1();
    struct table *table2 = create_test_table2();
    struct table *table3 = create_test_table3();

    add_table(db, table1);
    add_table(db, table2);
    add_table(db, table3);

    delete_table(db, "table1");
    delete_table(db, "table2");

    close_db(db);

    printf("table list test have passed\n");
}


static void time_init() {
    time_t t;
    srand((unsigned) time(&t));
}

// stress tests
void insert_stress_test(int num_of_elements) {
    time_init();

    char *db_name = my_malloc(DB_NAME_SIZE);
    memcpy(db_name, "insert_stress_test.txt", DB_NAME_SIZE);

    struct db *db = open_db(db_name);

    struct table *table1 = create_test_table1();

    add_table(db, table1);

    FILE *insert_time_file = fopen("insert_stress_test_time.txt", "w+");

    for (size_t i = 0; i < num_of_elements; i++) {
        clock_t start = clock();
        insert_to_table(db, table1->name, create_random_tuple(table1->num_of_columns, table1->table_scheme));
        double time = clock() - start;
        char str[20];
        sprintf(str, "%.10f", (double) time);
        fwrite(str, 10 * sizeof(char), 1, insert_time_file);
        char space = '\n';
        fwrite(&space, sizeof(char), 1, insert_time_file);
    }

    close_db(db);
}


//void select_stress_test(int num_of_elements, int num_of_selects) {
//    time_init();
//
//    char *db_name = my_malloc(DB_NAME_SIZE);
//    memcpy(db_name, "select_stress_test.txt", DB_NAME_SIZE);
//
//    struct db *db = open_db(db_name);
//
//    struct table *table = create_test_table2();
//
//    add_table(db, table);
//
//    FILE *select_time_file = fopen("select_stress_test_time.txt", "w+");
//
//    for (size_t i = 0; i < num_of_elements; i++) {
//        insert_to_table(db, table->name, create_random_tuple(table->num_of_columns, table->table_scheme));
//    }
//
//    // select all test
//    struct filter **null_filters = create_random_filters(0, table->num_of_columns, table->table_scheme);
//    select_from_table(db, table->name, 0, null_filters);
//
//    for (size_t i = 0; i < num_of_selects; i++) {
//        uint16_t num_of_filters = 1;
//
//        struct filter **filters = create_random_filters(num_of_filters, table->num_of_columns, table->table_scheme);
//
//        clock_t start = clock();
//
//        select_from_table(db, table->name, num_of_filters, filters);
//
//        double time = clock() - start;
//        char str[20];
//        sprintf(str, "%.10f", (double) time);
//        fwrite(str, 10 * sizeof(char), 1, select_time_file);
//        char space = '\n';
//        fwrite(&space, sizeof(char), 1, select_time_file);
//
//        free_filters(filters, 1);
//    }
//
//    close_db(db);
//}

// asymptotic tests
void delete_asymptotic_test(int max_num_of_elements) {
    if (max_num_of_elements <= 100) {
        max_num_of_elements = 100;
    }

    time_init();

    char *db_name = my_malloc(DB_NAME_SIZE);
    memcpy(db_name, "delete_asymptotic_test.txt", DB_NAME_SIZE);

    struct db *db = open_db(db_name);

    FILE *delete_asymptotic_time_file = fopen("delete_asymptotic_time.txt", "w+");

    uint16_t num_of_filters = 1;
    struct filter **filters = my_malloc(sizeof(struct filter *));

    struct filter *filter = my_malloc(sizeof(struct filter));
    filter->col_name = "float_column";
    filter->filter_type = CONST;
    filter->filter_cond = FLOAT_GR_EQ;
    float *value = my_malloc(sizeof(float));
    *value = 500;
    filter->value = value;
    filters[0] = filter;

    for (size_t i = 1; i <= max_num_of_elements; i++) {
        printf("%zu\n", i);

        struct table *table = create_test_table2();

        add_table(db, table);

        for (size_t j = 0; j < i; j++) {
            insert_to_table(db, table->name, create_random_tuple(table->num_of_columns, table->table_scheme));
        }

        clock_t start = clock();

        delete_from_table(db, table->name, num_of_filters, filters);

        double time = clock() - start;
        char str[20];
        sprintf(str, "%.10f", (double) time);
        fwrite(str, 10 * sizeof(char), 1, delete_asymptotic_time_file);
        char space = '\n';
        fwrite(&space, sizeof(char), 1, delete_asymptotic_time_file);

        delete_table(db, table->name);
    }

    my_free(value, sizeof(float));
    my_free(filter, sizeof(struct filter));
    my_free(filters, sizeof(struct filter *));

    close_db(db);
}


void update_asymptotic_test(int max_num_of_elements) {
    if (max_num_of_elements <= 100) {
        max_num_of_elements = 100;
    }

    time_init();

    char *db_name = my_malloc(DB_NAME_SIZE);
    memcpy(db_name, "update_asymptotic_test.txt", DB_NAME_SIZE);

    struct db *db = open_db(db_name);

    FILE *update_asymptotic_time_file = fopen("update_asymptotic_time.txt", "w+");

    uint16_t num_of_filters = 0;
    struct filter **filters = NULL;

    char *value = "qwerty";
    uint16_t value_size = strlen(value);

    uint16_t num_of_updates = 1;
    struct update_value *update_value = my_malloc(sizeof(struct update_value));
    update_value->attribute_num = 3;
    update_value->value = my_malloc(value_size * sizeof(char));
    memcpy(update_value->value, value, value_size);

    struct update_query *update_query = my_malloc(sizeof(struct update_query));
    update_query->num_of_updates = num_of_updates;
    update_query->update_values = my_malloc(num_of_updates * sizeof(struct update_value *));
    update_query->update_values[0] = update_value;

    for (size_t i = 1; i <= max_num_of_elements; i++) {
        printf("%zu\n", i);

        struct table *table = create_test_table3();

        add_table(db, table);

        for (size_t j = 0; j < i; j++) {
            insert_to_table(db, table->name, create_random_tuple(table->num_of_columns, table->table_scheme));
        }

        clock_t start = clock();

        update_table(db, table->name, num_of_filters, filters, update_query);

        double time = clock() - start;
        char str[20];
        sprintf(str, "%.10f", (double) time);
        fwrite(str, 10 * sizeof(char), 1, update_asymptotic_time_file);
        char space = '\n';
        fwrite(&space, sizeof(char), 1, update_asymptotic_time_file);

        if (i != max_num_of_elements) {
            delete_table(db, table->name);
        }
    }

    my_free(update_query->update_values, num_of_updates * sizeof(struct update_value *));
    my_free(update_query, sizeof(struct update_query));
    my_free(update_value->value, value_size * sizeof(char));
    my_free(update_value, sizeof(struct update_value));

    close_db(db);
}

