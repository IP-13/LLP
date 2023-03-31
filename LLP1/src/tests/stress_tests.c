#pragma once

#include <time.h>
#include "tests.h"


void insert_test() {
    time_t t;
    srand((unsigned) time(&t));

    char *db_name = my_malloc(DB_NAME_SIZE);
    memcpy(db_name, "insert_stress_test.txt", DB_NAME_SIZE);

    struct db *db = open_db(db_name);

    struct table *table1 = create_test_table1();

    add_table(db, table1);

    FILE *insert_time = fopen("insert_stress_test_time.txt", "w+");

    for (size_t i = 0; i < 10000000; i++) {
        clock_t start = clock();
        insert_to_table(db, table1->name, create_random_tuple(table1->num_of_columns, table1->table_scheme));
        double time = clock() - start;
        char *str[20];
        sprintf(str, "%.10f", (double) time);
        fwrite(str, 10 * sizeof(char), 1, insert_time);
        char space = '\n';
        fwrite(&space, sizeof(char), 1, insert_time);
    }

    close_db(db);
}

