#ifndef __TYPES_HPP
#define __TYPES_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include <cstdlib>
#include <cstdio>

#include "db.hpp"

#define BUF_READ_SIZE 1024

typedef struct InputBuffer {
    char* buf;
    size_t len;
    size_t pos;
} InputBuffer;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct Statement {
    StatementType type;
    Row row_to_insert;
} Statement;

static struct InputBuffer* new_input_buffer()
{
    struct InputBuffer* input_buffer = (struct InputBuffer*)malloc(sizeof(struct InputBuffer));
    if (input_buffer == nullptr)
        exit(EXIT_FAILURE);

    input_buffer->buf = nullptr;
    input_buffer->len = 0;
    input_buffer->pos = 0;

    return input_buffer;
}

static void close_input_buffer(struct InputBuffer* input_buffer)
{
    free(input_buffer->buf);
    free(input_buffer);
}

#ifdef __cplusplus
}
#endif

#endif /*__TYPES_HPP*/