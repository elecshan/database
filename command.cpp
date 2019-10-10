#include "command.hpp"
#include "types.hpp"

#include <cstring>
#include <sys/types.h>
#include <regex.h>

MetaCommandResult do_meta_command(InputBuffer* buffer, Table* table)
{
    if (strcmp(buffer->buf, ".exit") == 0) {
        close_input_buffer(buffer);
        db_close(table);
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

bool check_email_format(char* email)
{
    const char* regex = "^[a-zA-Z0-9_-]+@[a-zA-Z0-9_-]+(\\.[a-zA-Z0-9_-]+)+$"; /* + 是扩展正则表达式的一部分*/
    regex_t preg;

    int ret;
    if (ret = regcomp(&preg, regex, REG_NOSUB | REG_NEWLINE | REG_EXTENDED) != 0) {
        printf("Error: regcomp error %d.\n", ret);
        exit(EXIT_FAILURE);
    }
    return regexec(&preg, email, 0, NULL, 0) == 0;
    // regfree(preg);
    // return r;
}

PrepareResult prepare_insert(InputBuffer* buffer, Statement* statement)
{
    statement->type = STATEMENT_INSERT;
    
    char* keyword = strtok(buffer->buf, " ");
    char* id_str = strtok(NULL, " ");
    char* username = strtok(NULL, " ");
    char* email = strtok(NULL, " ");

    if (id_str == nullptr || username == nullptr || email == nullptr)
        return PREPARE_SYNTAX_ERROR;
    
    if (strlen(username) > COLUMN_USERNAME_SIZE || strlen(email) > COLUMN_EMAIL_SIZE)
        return PREPARE_STRING_TOO_LONG;
    if (atoi(id_str) < 0)
        return PREPARE_NEGATIVE_ID;
    if (!check_email_format(email))
        return PREPARE_EMAIL_ERROR;

    statement->row_to_insert.id = atoi(id_str);
    strcpy(statement->row_to_insert.username, username);
    strcpy(statement->row_to_insert.email, email);

    return PREPARE_SUCCESS;
}

PrepareResult prepare_statement(InputBuffer* buffer, Statement* statement)
{
    if (strncmp(buffer->buf, "insert", 6) == 0) {
        return prepare_insert(buffer, statement);
    }
    if (strncmp(buffer->buf, "select", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_statement(Statement* statement, Table* table)
{
    switch (statement->type)
    {
    case (STATEMENT_INSERT):
        return execute_insert(statement, table);
    case (STATEMENT_SELECT):
        return execute_select(statement, table);    
    }
}

ExecuteResult execute_insert(Statement* statement, Table* table)
{
    if (table->num_rows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    Row* row_to_insert = &(statement->row_to_insert);
    serialize_row(row_to_insert, row_slot(table, table->num_rows));
    table->num_rows++;

    return EXECUTE_SUCCESS;
}

void print_row(Row* row)
{
    printf("%d, %s, %s\n", row->id, row->username, row->email);
}

ExecuteResult execute_select(Statement* statement, Table* table)
{
    Row row;
    for (uint32_t i = 0; i < table->num_rows; i++) {
        deserialize_row(row_slot(table, i), &row);
        print_row(&row);
    }

    return EXECUTE_SUCCESS;
}