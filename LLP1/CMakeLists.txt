cmake_minimum_required(VERSION 3.22)
project(LLP1)

include_directories(include)

add_executable(main
        main.c
        src/db/column.c
        src/db/db.c
        src/db/filters.c
        src/db/page.c
        src/db/table.c
        src/db/tuple.c
        src/tests/tests.c
        src/util/launch_db.c
        src/util/my_malloc.c
        src/util/util.c
        )

