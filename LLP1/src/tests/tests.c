#pragma once

#include "tests.h"


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

