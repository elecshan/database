#include <iostream>
#include <cstdio>
#include <cstring>

#include "sds.hpp"
#include "read.hpp"

void print_prompt()
{
    printf("db > ");
}

int main()
{
    dbReader* input_buffer = new dbReader();

    while(true) {
        print_prompt();
        input_buffer->sread(std::cin);

        if (input_buffer->strcmpdb(".exit") == 0) {
            exit(EXIT_SUCCESS);
        }
        else {
            printf("Unrecognized command ");
            input_buffer->printdb();
            printf(".\n");
        }
    }
}