#ifndef __COMMAND_HPP
#define __COMMAND_HPP

#include "types.hpp"
#include "db.hpp"

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_STRING_TOO_LONG,
    PREPARE_NEGATIVE_ID,
    PREPARE_EMAIL_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
} ExecuteResult;

MetaCommandResult do_meta_command(InputBuffer*, Table*);
PrepareResult prepare_statement(InputBuffer*, Statement*);

ExecuteResult execute_statement(Statement*, Table*);
ExecuteResult execute_insert(Statement*, Table*);
ExecuteResult execute_select(Statement*, Table*);


#endif /*__COMMAND_HPP*/