#include <iostream>
#include <cstdio>
#include <cstring>

#include "sds.hpp"
#include "types.hpp"
#include "command.hpp"
#include "db.hpp"

void print_prompt()
{
    printf("db > ");
}

void read_input(InputBuffer* buffer)
{
    int pos = 0, len = BUF_READ_SIZE;
    buffer->buf = (char*)malloc(sizeof(char) * BUF_READ_SIZE);
    if (buffer->buf == nullptr)
        exit(EXIT_FAILURE);
    
    char c;
    while((c = std::cin.get()) != '\n') {
        buffer->buf[pos] = c;
        if (pos >= len - 1) {
            len += BUF_READ_SIZE;
            char* tmp = (char*)realloc((void*)buffer->buf, len);
            if (tmp == nullptr) {
                free(buffer->buf);
                exit(EXIT_FAILURE);
            }
            buffer->buf = tmp;
        }
        pos++;
    }

    // if (pos <= 0) {
    //     printf("Error reading input\n");
    //     exit(EXIT_FAILURE);
    // }

    buffer->buf[pos++] = '\0';
    buffer->len = len;
    buffer->pos = pos;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Must supply a database filename.\n");
        exit(EXIT_FAILURE);
    }

    char* filename = argv[1];
    InputBuffer* input_buffer = new_input_buffer();
    Table* table = db_open(filename);

    while(true) {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buf == nullptr || input_buffer->buf[0] == '\0')
            continue;

        if (input_buffer->buf[0] == '.') {
            switch (do_meta_command(input_buffer, table))
            {
            case (META_COMMAND_SUCCESS):
                continue;
            case (META_COMMAND_UNRECOGNIZED_COMMAND):
                printf("Unrecognized command '%s'\n", input_buffer->buf);
                continue;
            default:
                break;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement))
        {
        case (PREPARE_SUCCESS):
            break;
        case (PREPARE_SYNTAX_ERROR):
            printf("Syntax error. Could not parse statement.\n");
            continue;
        case (PREPARE_STRING_TOO_LONG):
            printf("String is too long.\n");
            continue;
        case (PREPARE_NEGATIVE_ID):
            printf("ID must be positive.\n");
            continue;
        case (PREPARE_EMAIL_ERROR):
            printf("Email address error.\n");
            continue;
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buf);
            continue;
        default:
            break;
        }

        switch (execute_statement(&statement, table))
        {
        case (EXECUTE_SUCCESS):
            printf("Excuted.\n");
            break;
        case (EXECUTE_TABLE_FULL):
            printf("ERROR: Table full.\n");
            break;
        }
    }
}