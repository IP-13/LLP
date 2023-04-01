cmake_minimum_required(VERSION 3.22)
project(LLP1)

include_directories(include)

add_executable(main
        main.c
        src/db/db.c
        src/util/my_malloc.c
        src/util/vector.c
        src/util/table_list.c
        src/util/column_list.c
        src/util/launch_db.c
        src/util/util.c
        src/page/page.c
        src/tuple/tuple.c
        src/tuple/filters.c
        src/table/table.c
        src/tests/tests.c
        src/tests/stress_tests.c
        include/is_test.h)