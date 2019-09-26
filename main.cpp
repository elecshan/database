#include <iostream>
#include <cstdio>
#include <cstring>

#include "sds.hpp"
#include "types.hpp"
#include "command.hpp"

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

    if (pos <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    buffer->buf[pos++] = '\0';
    buffer->len = len;
    buffer->pos = pos;
}

int main()
{
    InputBuffer* input_buffer = new_input_buffer();

    while(true) {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buf == nullptr)
            continue;

        if (input_buffer->buf[0] == '.') {
            switch (do_meta_command(input_buffer))
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
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buf);
            continue;
        default:
            break;
        }

        execute_statement(&statement);
        printf("Excuted.\n");
    }
}