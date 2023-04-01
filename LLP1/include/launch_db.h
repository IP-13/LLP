#pragma once

#include "util.h"

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

