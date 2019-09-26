#ifndef __COMMAND_HPP
#define __COMMAND_HPP

#include "types.hpp"

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

MetaCommandResult do_meta_command(InputBuffer*);
PrepareResult prepare_statement(InputBuffer*, Statement*);
void execute_statement(Statement*);

#endif /*__COMMAND_HPP*/