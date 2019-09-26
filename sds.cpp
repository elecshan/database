#include "sds.hpp"
#include <cstring>

sds::sds()
{
    buf = (char*)malloc(1);
    if (buf == nullptr)
        exit(EXIT_FAILURE);
    buf[0] = '\0';
    len = 0;
    alloc = 0;
}

sds::sds(const void* init, size_t initlen)
{
    buf = (char*)malloc(initlen + 1);
    if (buf == nullptr)
        exit(EXIT_FAILURE);

    if (!init)
        memcpy(buf, init, initlen);

    len = initlen;
    alloc = initlen;
    buf[initlen] = '\0';
}

sds::sds(const char* init)
{
    size_t len = (init == nullptr) ? 0 : strlen(init);
    sds(init, len);
}

sds::~sds()
{
    sdsfree();
}

sds& sds::operator=(const sds& other)
{
    sdscpy(other.buf);
    return *this;
}

void sds::sdsfree()
{
    if (buf == nullptr) return;
    free(buf);
}

void sds::sdscpylen(const char* t, size_t len)
{
    if (this->alloc < len) {
        free(buf);
        buf = (char*)malloc(len + 1);
        this->alloc = len;
    }
    memcpy(buf, t, len);
    buf[len] = '\0';
    this->len = len;
}

void sds::sdscpy(const char* t)
{
    sdscpylen(t, strlen(t));
}