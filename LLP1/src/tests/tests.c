#pragma once

#include "tests.h"


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
    memcpy(table1_name, "first table", table1_name_size);
    uint64_t table1_num_of_columns = 3;
    struct table *table1 = create_table(table1_name_size, table1_name, table1_num_of_columns, column_list1);

    return table1;
}

struct table *create_test_table2() {
    struct column_list *column_list2 = create_column_list();
    struct column *col2_1 = my_malloc(sizeof(struct column));
    col2_1->data_type = STRING;
    col2_1->name_size = 6;
    col2_1->name = my_malloc(col2_1->name_size * sizeof(char));
    memcpy(col2_1->name, "digits", col2_1->name_size);
    struct column *col2_2 = my_malloc(sizeof(struct column));
    col2_2->data_type = STRING;
    col2_2->name_size = 4;
    col2_2->name = my_malloc(col2_2->name_size * sizeof(char));
    memcpy(col2_2->name, "1234", col2_2->name_size);
    struct column *col2_3 = my_malloc(sizeof(struct column));
    col2_3->data_type = STRING;
    col2_3->name_size = 9;
    col2_3->name = my_malloc(col2_3->name_size * sizeof(char));
    memcpy(col2_3->name, "col3_3 name", col2_3->name_size);
    struct column *col2_4 = my_malloc(sizeof(struct column));
    col2_4->data_type = STRING;
    col2_4->name_size = 11;
    col2_4->name = my_malloc(col2_4->name_size * sizeof(char));
    memcpy(col2_4->name, "qwerty_name", col2_4->name_size);
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
    memcpy(col3_1->name, "digits", col3_1->name_size);
    struct column *col3_2 = my_malloc(sizeof(struct column));
    col3_2->data_type = STRING;
    col3_2->name_size = 4;
    col3_2->name = my_malloc(col3_2->name_size * sizeof(char));
    memcpy(col3_2->name, "1234", col3_2->name_size);
    struct column *col3_3 = my_malloc(sizeof(struct column));
    col3_3->data_type = STRING;
    col3_3->name_size = 9;
    col3_3->name = my_malloc(col3_3->name_size * sizeof(char));
    memcpy(col3_3->name, "col3_3 name", col3_3->name_size);
    struct column *col3_4 = my_malloc(sizeof(struct column));
    col3_4->data_type = STRING;
    col3_4->name_size = 11;
    col3_4->name = my_malloc(col3_4->name_size * sizeof(char));
    memcpy(col3_4->name, "qwerty_name", col3_4->name_size);
    column_list_push(column_list3, col3_1);
    column_list_push(column_list3, col3_2);
    column_list_push(column_list3, col3_3);
    column_list_push(column_list3, col3_4);

    file_offset table3_offset;
    table3_offset.offset = 3 * PAGE_SIZE;
    uint64_t table3_name_size = 10;
    char *table3_name = my_malloc(table3_name_size);
    memcpy(table3_name, "last table", table3_name_size);
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
        s[i] = (char) ('a' + rand() % 26);
    }

    return s;
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
                int_field->data = rand() % 100000;
                data[i] = int_field;
                break;
            }
            case FLOAT : {
                struct float_field *float_field = my_malloc(sizeof(struct float_field));
                float_field->data = (rand() % 100) / 200;
                data[i] = float_field;
                break;
            }
            case STRING: {
                struct string_field *string_field = my_malloc(sizeof(struct string_field));
                string_field->size = (rand() % 20) + 1;
                string_field->data = create_random_string(string_field->size);
                data[i] = string_field;
                break;
            }
        }
    }

    return create_tuple((page_offset) {.offset = 0}, data, num_of_attributes, table_scheme);
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

void test_column_list() {
    struct column_list *column_list = create_column_list();
    struct column *col1 = my_malloc(sizeof(struct column));
    col1->data_type = INT;
    col1->name_size = 16;
    col1->name = my_malloc(col1->name_size * sizeof(char));
    memcpy(col1->name, "I LIKE THIS NAME", col1->name_size);
    struct column *col2 = my_malloc(sizeof(struct column));
    col2->data_type = FLOAT;
    col2->name_size = 10;
    col2->name = my_malloc(col2->name_size * sizeof(char));
    memcpy(col2->name, "name of 10", col2->name_size);
    struct column *col3 = my_malloc(sizeof(struct column));
    col3->data_type = STRING;
    col3->name_size = 9;
    col3->name = my_malloc(col3->name_size * sizeof(char));
    memcpy(col3->name, "col3 name", col3->name_size);

    column_list_push(column_list, col1);
    column_list_push(column_list, col2);
    column_list_push(column_list, col3);

    printf("name: %s\n", column_list_get(column_list, 2)->value->name);

    column_list_remove(&column_list, 2);

    column_list_clear(column_list);

    printf("column list test have passed\n");
}

void test_table_list() {
    FILE *file = fopen("test_table_list.txt", "w+");

    struct table *table1 = create_test_table1();
    struct table *table2 = create_test_table2();
    struct table *table3 = create_test_table3();

    struct table_list *table_list = create_table_list();

    table_list_push(table_list, table1);
    table_list_push(table_list, table2);
    table_list_push(table_list, table3);

    show_table_list(table_list);

    show_table(table_list_get(table_list, 1)->value);

    table_list_remove(&table_list, 2);
    table_list_remove(&table_list, 0);

    show_table_list(table_list);

    table_list_clear(table_list);

    printf("table list test have passed\n");
}

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

void tuple_test() {
    FILE *file = fopen("tuple_test.txt", "w+");

    uint32_t num_of_attributes = 4;

    enum data_type *table_scheme = my_malloc(num_of_attributes * sizeof(enum data_type));
    table_scheme[0] = BOOL;
    table_scheme[1] = INT;
    table_scheme[2] = FLOAT;
    table_scheme[3] = STRING;


    page_offset start;
    start.offset = 10;

    void **data = my_malloc(num_of_attributes * sizeof(void *));

    data[0] = my_malloc(sizeof(struct bool_field));
    ((struct bool_field *) data[0])->data = 1;

    data[1] = my_malloc(sizeof(struct int_field));
    ((struct int_field *) data[1])->data = 2;

    data[2] = my_malloc(sizeof(struct float_field));
    ((struct float_field *) data[2])->data = 11.123123F;

    data[3] = my_malloc(sizeof(struct string_field));
    ((struct string_field *) data[3])->data = my_malloc(10 * sizeof(char));
    ((struct string_field *) data[3])->size = 10;
    memcpy(((struct string_field *) data[3])->data, "10charline", 10);


    struct tuple *tuple = create_tuple(start, data, num_of_attributes, table_scheme);

    write_tuple(tuple, num_of_attributes, table_scheme, file);


    fseek(file, 0, SEEK_SET);

    struct tuple *tuple1 = read_tuple(num_of_attributes, table_scheme, file);

    print_tuple(tuple, num_of_attributes, table_scheme);
}

void page_test() {
    FILE *file = fopen("page_test.txt", "w+");

    uint32_t num_of_attributes = 4;

    enum data_type *table_scheme = my_malloc(num_of_attributes * sizeof(enum data_type));
    table_scheme[0] = BOOL;
    table_scheme[1] = INT;
    table_scheme[2] = FLOAT;
    table_scheme[3] = STRING;

    page_offset start1;
    start1.offset = 10;
    void **data1 = my_malloc(num_of_attributes * sizeof(void *));
    data1[0] = my_malloc(sizeof(struct bool_field));
    ((struct bool_field *) data1[0])->data = 1;
    data1[1] = my_malloc(sizeof(struct int_field));
    ((struct int_field *) data1[1])->data = 2;
    data1[2] = my_malloc(sizeof(struct float_field));
    ((struct float_field *) data1[2])->data = 11.123123F;
    data1[3] = my_malloc(sizeof(struct string_field));
    ((struct string_field *) data1[3])->data = my_malloc(10 * sizeof(char));
    ((struct string_field *) data1[3])->size = 10;
    memcpy(((struct string_field *) data1[3])->data, "10charline", ((struct string_field *) data1[3])->size);
    struct tuple *tuple1 = create_tuple(start1, data1, num_of_attributes, table_scheme);


    page_offset start2;
    start2.offset = 10;
    void **data2 = my_malloc(num_of_attributes * sizeof(void *));
    data2[0] = my_malloc(sizeof(struct bool_field));
    ((struct bool_field *) data2[0])->data = 1;
    data2[1] = my_malloc(sizeof(struct int_field));
    ((struct int_field *) data2[1])->data = 2;
    data2[2] = my_malloc(sizeof(struct float_field));
    ((struct float_field *) data2[2])->data = 11.123123F;
    data2[3] = my_malloc(sizeof(struct string_field));
    ((struct string_field *) data2[3])->data = my_malloc(10 * sizeof(char));
    ((struct string_field *) data2[3])->size = 5;
    memcpy(((struct string_field *) data2[3])->data, "string", ((struct string_field *) data2[3])->size);
    struct tuple *tuple2 = create_tuple(start2, data2, num_of_attributes, table_scheme);


    page_offset start3;
    start3.offset = 10;
    uint16_t size3 = 10;
    void **data3 = my_malloc(num_of_attributes * sizeof(void *));
    data3[0] = my_malloc(sizeof(struct bool_field));
    ((struct bool_field *) data3[0])->data = 1;
    data3[1] = my_malloc(sizeof(struct int_field));
    ((struct int_field *) data3[1])->data = 2;
    data3[2] = my_malloc(sizeof(struct float_field));
    ((struct float_field *) data3[2])->data = 11.123123F;
    data3[3] = my_malloc(sizeof(struct string_field));
    ((struct string_field *) data3[3])->data = my_malloc(10 * sizeof(char));
    ((struct string_field *) data3[3])->size = 5;
    memcpy(((struct string_field *) data3[3])->data, "string", ((struct string_field *) data3[3])->size);
    struct tuple *tuple3 = create_tuple(start3, data3, num_of_attributes, table_scheme);


    page_offset start4;
    start4.offset = 10;
    uint16_t size4 = 10;
    void **data4 = my_malloc(num_of_attributes * sizeof(void *));
    data4[0] = my_malloc(sizeof(struct bool_field));
    ((struct bool_field *) data4[0])->data = 1;
    data4[1] = my_malloc(sizeof(struct int_field));
    ((struct int_field *) data4[1])->data = 2;
    data4[2] = my_malloc(sizeof(struct float_field));
    ((struct float_field *) data4[2])->data = 11.123123F;
    data4[3] = my_malloc(sizeof(struct string_field));
    ((struct string_field *) data4[3])->data = my_malloc(10 * sizeof(char));
    ((struct string_field *) data4[3])->size = 5;
    memcpy(((struct string_field *) data4[3])->data, "string", ((struct string_field *) data4[3])->size);
    struct tuple *tuple4 = create_tuple(start4, data4, num_of_attributes, table_scheme);


    file_offset offset;
    offset.offset = PAGE_SIZE;

    file_offset prev_page;
    prev_page.offset = PAGE_SIZE * 8;

    file_offset next_page;
    next_page.offset = PAGE_SIZE * 3;

    struct page *page = create_page(offset, prev_page, next_page);

    page->tuples[0] = tuple1;
    page->tuples[1] = tuple2;
    page->tuples[2] = tuple3;
    page->tuples[3] = tuple4;
    page->num_of_tuples = 4;

    write_page(page, num_of_attributes, table_scheme, file);

    struct page *page2 = read_page(file, offset, table_scheme, num_of_attributes);

    print_page(page2, num_of_attributes, table_scheme);
}

