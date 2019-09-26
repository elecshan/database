#ifndef __SDS_HPP
#define __SDS_HPP

#include <iostream>

// typedef char* sds;

// struct __attribute__((__packed__)) sdshdr {
//     uint32_t len;
//     uint32_t alloc;
//     char buf[];
// };

// sds sdsnewlen(const void* init, size_t initlen);
// sds sdsnew(const char* init);
// sds sdsempty();
// void sdsfree(sds s);
// sds sdscpylen(sds s, const char* t, size_t len);
// sds sdscpy(sds s, const char* t);

class sds {
public:
    sds();
    sds(const void* init, size_t initlen);
    sds(const char* init);
    ~sds();

    sds& operator=(const sds& other);

    void sdscpy(const char* t);
    void sdscpylen(const char* t, size_t len);

private:
    void sdsfree();

private:
    uint32_t len; /*used*/
    uint32_t alloc; /* excluding null terminator */
    char* buf;
};

#endif /*__SDS_HPP*/