#pragma once

#include <inttypes.h>
#include <string.h>
#include "tuple.h"
#include "column_list.h"
#include "table.h"
#include "db.h"
#include "structures.h"
#include "my_malloc.h"
#include "util.h"


enum command_type {
    OPEN_DB = 0,
    CREATE_DB,
    CLOSE_DB,
    CREATE_TABLE,
    DELETE_TABLE,
    INSERT,
    SELECT,
    DELETE,
    UPDATE,
    JOIN,
    EXIT
};

void launch_db();

