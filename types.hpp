#ifndef __TYPES_HPP
#define __TYPES_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include <cstdlib>
#include <cstdio>

#define BUF_READ_SIZE 1024

typedef struct InputBuffer {
    char* buf;
    size_t len;
    size_t pos;
} InputBuffer;

struct InputBuffer* new_input_buffer()
{
    struct InputBuffer* input_buffer = (struct InputBuffer*)malloc(sizeof(struct InputBuffer));
    input_buffer->buf = nullptr;
    input_buffer->len = 0;
    input_buffer->pos = 0;

    return input_buffer;
}

void close_input_buffer(struct InputBuffer* input_buffer)
{
    free(input_buffer->buf);
    free(input_buffer);
}

#ifdef __cplusplus
}
#endif

#endif /*__TYPES_HPP*/