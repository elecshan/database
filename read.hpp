#ifndef __READ_HPP
#define __READ_HPP

#define BUF_READ_SIZE 1024

// #include <cstdio>
// #include <iostream>
#include <istream>

class dbReader {
public:
    dbReader();
    ~dbReader();

    void sread(std::istream& s);
    int strcmpdb(const char* str);
    void printdb();

private:
    int err; /*Error type, 0 when there is no error.*/
    char errstr[128]; /*Error message*/

    char* buf;
    size_t len; /*Buffer length (include unused buffer)*/
    size_t pos; /*Buffer cursor points to '\0'*/
};

#endif /*__READ_HPP*/