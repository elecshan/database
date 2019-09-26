#include <cstring>
#include "read.hpp"

dbReader::dbReader()
{
    err = 0;
    errstr[0] = '\0';
    buf = (char*)malloc(sizeof(char) * BUF_READ_SIZE);
    len = BUF_READ_SIZE;
    pos = 0;
}

dbReader::~dbReader()
{
    if (buf != nullptr)
        free(buf);
}

void dbReader::sread(std::istream& s)
{
    pos = 0;
    char c;
    while((c = s.get()) != '\n') {
        buf[pos] = c;
        if (pos >= len - 1) {
            len = strlen(buf) + BUF_READ_SIZE;
            char* tmp = (char*)realloc((void*)buf, len);
            if (tmp == nullptr) {
                free(buf);
                exit(EXIT_FAILURE);
            }
            buf = tmp;
        }
        pos++;
    }

    if (pos <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    buf[pos++] = '\0';
}

int dbReader::strcmpdb(const char* str)
{
    return strcmp(buf, str);
}

void dbReader::printdb()
{
    printf("%s", buf);
}