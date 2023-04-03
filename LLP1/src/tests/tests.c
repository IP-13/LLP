#pragma once

#include "tests.h"

#define INT_RANGE 1000000
#define FLOAT_RANGE 1000
#define RANDOM_STRING_SIZE 20

// tests util
static struct table *create_test_table1() {
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

static struct table *create_test_table2() {
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

static struct table *create_test_table3() {
    struct column_list *column_list3 = create_column_list();
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

static void show_table(struct table *table) {
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

static void show_table_list(struct table_list *table_list) {
    int i = 1;
    while (table_list->next != NULL) {
        printf("table %d", i++);
        show_table(table_list->value);
        table_list = table_list->next;
    }

    printf("table %d", i++);
    show_table(table_list->value);
}

static void print_page(struct page *page, uint32_t num_of_attributes, enum data_type *table_scheme) {
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

static struct tuple *create_random_tuple(uint64_t num_of_attributes, const enum data_type *table_scheme) {
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

static struct filter *create_random_filter(uint64_t num_of_attributes, const enum data_type *table_scheme) {
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

static struct filter **create_random_filters(uint16_t num_of_filters, uint64_t num_of_attrs, enum data_type *table_scheme) {
    struct filter **filters = my_malloc(num_of_filters * sizeof(struct filter *));

    for (size_t i = 0; i < num_of_filters; i++) {
        filters[i] = create_random_filter(num_of_attrs, table_scheme);
    }

    return filters;
}

static void print_db_scheme(struct db *db) {
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

// module tests
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
        char str[20];
        sprintf(str, "%.10f", (double) time);
        fwrite(str, 10 * sizeof(char), 1, select_time_file);
        char space = '\n';
        fwrite(&space, sizeof(char), 1, select_time_file);

        free_filters(filters, 1);
    }

    close_db(db);
}

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

