#pragma once

#include "launch_db.h"


void launch_db() {
    printf("Enter db name (%d or less symbols): ", DB_NAME_SIZE);
    char *db_name = my_malloc(20 * sizeof(char));
    scanf("%s", db_name);

    struct db *db = open_db(db_name);

    enum command_type command = 0;

    while (command != EXIT) {
        printf("Available:\n0. create table\n1. delete table\n2. insert\n3. select\n4. delete\n5. update_table\n6. join\n7. exit\n");
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
                char *table_name = scanf_table_name();
                uint16_t num_of_filters = scanf_num_of_filters();
                struct filter **filters = scanf_filters(num_of_filters);

                struct table_list *table_list = table_list_get(
                        db->table_list, get_table_index_by_name(db->table_list, table_name));

                if (table_list == NULL) {
                    break;
                }

                struct table *table = table_list->value;

                if (table == NULL) {
                    break;
                }

                struct update_query *update_query = scanf_update_query(table->table_scheme);

                update_table(db, table_name, num_of_filters, filters, update_query);

                free_update_query(update_query, table->table_scheme);
                free_filters(filters, num_of_filters);
                my_free(table_name, TABLE_NAME_SIZE);
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