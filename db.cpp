#include "db.hpp"

#include <cstdlib>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

Pager* pager_open(const char* filename)
{
    int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
    if (fd == -1) {
        printf("Open file failure.\n");
        exit(EXIT_FAILURE);
    }

    off_t offset = lseek(fd, 0, SEEK_END);
    Pager* pager = (Pager*)malloc(sizeof(Pager));
    pager->fd = fd;
    pager->file_len = offset;

    for (uint32_t i = 0; i < TABLE_MAX_PAGE; i++)
        pager->pages[i] = nullptr;

    return pager;
}

void pager_flush(Pager* pager, uint32_t page_num, uint32_t size)
{
    if (pager->pages[page_num] == nullptr) {
        printf("Warning: Try to flush empty page.\n");
        return;
    }

    off_t offset = lseek(pager->fd, page_num * PAGE_SIZE, SEEK_SET);
    if (offset == -1) {
        printf("Error: Seek file: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes = write(pager->fd, pager->pages[page_num], size);
    if (bytes == -1) {
        printf("Error: Write file: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}

Table* db_open(const char* filename)
{
    Pager* pager = pager_open(filename);
    Table* table = (Table*)malloc(sizeof(Table));
    table->pager = pager;
    table->num_rows = pager->file_len / ROW_SIZE;
    return table;
}

void db_close(Table* table)
{
    Pager* pager = table->pager;
    uint32_t full_pages = table->num_rows / ROWS_PER_PAGE;

    for (uint32_t i = 0; i < full_pages; i++) {
        if (pager->pages[i] != nullptr) {
            pager_flush(pager, i, PAGE_SIZE);
            free(pager->pages[i]);
            pager->pages[i] = nullptr;
        }
    }

    uint32_t addition_rows = table->num_rows % ROWS_PER_PAGE;
    if (addition_rows > 0 && pager->pages[full_pages] != nullptr) {
        pager_flush(pager, full_pages, addition_rows * ROW_SIZE);
        free(pager->pages[addition_rows]);
        pager->pages[addition_rows] = nullptr;
    }

    if (close(pager->fd) == -1) {
        printf("Error: close db file.\n");
        exit(EXIT_FAILURE);
    }

    free(pager);
    free(table);
}

void* get_page(Pager* pager, uint32_t page_num)
{
    if (page_num >= TABLE_MAX_PAGE) {
        printf("Page number out of bound. %d\n", page_num);
        exit(EXIT_FAILURE);
    }

    if (pager->pages[page_num] == nullptr) {
        uint32_t pages = pager->file_len / PAGE_SIZE;
        pages = pages + (pager->file_len % PAGE_SIZE == 0 ? 0 : 1);

        void* page = malloc(PAGE_SIZE);
        if (page == nullptr) {
            printf("Error: Page allocate failure.\n");
            exit(EXIT_FAILURE);
        }

        if (page_num < pages) {
            lseek(pager->fd, page_num * PAGE_SIZE, SEEK_SET);
            ssize_t bytes = read(pager->fd, page, PAGE_SIZE);
            if (bytes == -1) {
                printf("Error: Read file failure.\n");
                exit(EXIT_FAILURE);
            }
        }
        pager->pages[page_num] = page;
    }

    return pager->pages[page_num];
}

void* row_slot(Table* table, uint32_t row_num)
{
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    void* page = get_page(table->pager, page_num);

    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t bytes_offset = row_offset * ROW_SIZE;
    return page + bytes_offset;
}