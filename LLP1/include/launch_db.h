#pragma once

#include <stdlib.h>
#include "my_malloc.h"
#include "db.h"
#include "tests.h"
#include "column_list.h"
#include "table_list.h"
#include "table.h"
#include "filter_type.h"

enum command_type {
    CREATE_TABLE = 0,
    DELETE_TABLE,
    INSERT,
    SELECT,
    DELETE,
    UPDATE,
    JOIN,
    EXIT
};

void launch_db();

