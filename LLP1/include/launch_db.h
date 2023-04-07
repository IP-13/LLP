#pragma once

#include <inttypes.h>
#include <string.h>
#include "tuple.h"
#include "table.h"
#include "db.h"
#include "my_malloc.h"
#include "util.h"

#define MAX_STR_SIZE 1024
#define TABLE_NAME_SIZE 20
#define COLUMN_NAME_SIZE 20
#define DB_NAME_SIZE 40


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

