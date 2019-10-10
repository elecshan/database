#ifndef __DB_HPP
#define __DB_HPP

#include <stdint.h>
#include <cstring>
#include <cstdio>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

#define TABLE_MAX_PAGE 100

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE + 1]; /*增加一个字节来存放C字符串的结束符*/
    char email[COLUMN_EMAIL_SIZE + 1];
} Row;

const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

void serialize_row(Row* source, void* destination);
void deserialize_row(void* source, Row* destination);

const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGE;

typedef struct Pager {
    int fd;
    uint32_t file_len;
    void* pages[TABLE_MAX_PAGE];
} Pager;

typedef struct Table {
    uint32_t num_rows;
    Pager* pager;
} Table;

Table* db_open(const char* filename);
void db_close(Table* table);

void* row_slot(Table* table, uint32_t row_num);

#ifdef __cplusplus
}
#endif

#endif /*__DB_HPP*/