#pragma once

#include <time.h>
#include "tests.h"


void time_init() {
    time_t t;
    srand((unsigned) time(&t));
}


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
        char *str[20];
        sprintf(str, "%.10f", (double) time);
        fwrite(str, 10 * sizeof(char), 1, insert_time_file);
        char space = '\n';
        fwrite(&space, sizeof(char), 1, insert_time_file);
    }

    close_db(db);
}


void select_stress_test(int num_of_elements, int num_of_selects) {
    time_init();

    char *db_name = my_malloc(DB_NAME_SIZE);
    memcpy(db_name, "select_stress_test.txt", DB_NAME_SIZE);

    struct db *db = open_db(db_name);

    struct table *table = create_test_table2();

    add_table(db, table);

    FILE *select_time_file = fopen("select_stress_test_time.txt", "w+");

    for (size_t i = 0; i < num_of_elements; i++) {
        insert_to_table(db, table->name, create_random_tuple(table->num_of_columns, table->table_scheme));
    }

    // select all test
    struct filter **null_filters = create_random_filters(0, table->num_of_columns, table->table_scheme);
    select_from_table(db, table->name, 0, null_filters);

    for (size_t i = 0; i < num_of_selects; i++) {
        uint16_t num_of_filters = 1;

        struct filter **filters = create_random_filters(num_of_filters, table->num_of_columns, table->table_scheme);

        clock_t start = clock();

        select_from_table(db, table->name, num_of_filters, filters);

        double time = clock() - start;
        char *str[20];
        sprintf(str, "%.10f", (double) time);
        fwrite(str, 10 * sizeof(char), 1, select_time_file);
        char space = '\n';
        fwrite(&space, sizeof(char), 1, select_time_file);

        free_filters(filters, 1);
    }

    close_db(db);
}


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
    filter->attribute_num = 0;
    filter->filter_type = FLOAT_GR_EQ;
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
        char *str[20];
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


void update_asymptotic_test(int num_of_elements) {

}

