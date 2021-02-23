#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>

enum
{
    ARGC_SUCCESS = 4,
    DEC = 10,
    ARG_ROWS = 2,
    ARG_COLS = 3,
    RIGHTS = 0664
};

int
main(int argc, char **argv)
{
    if(argc != ARGC_SUCCESS) {
        fprintf(stderr, "error: only three arguments should be passed.");
        return 1;
    }
    int rows = strtol(argv[ARG_ROWS], NULL, DEC);
    int cols = strtol(argv[ARG_COLS], NULL, DEC);
    char *path = argv[1];
    int fd = open(path, O_RDWR | O_CREAT, RIGHTS);
    if(fd < 0) {
        fprintf(stderr, "open(%s): %s\n", path, strerror(errno));
        exit(1);
    }
    size_t to_visit = rows * cols;
    size_t size = to_visit * sizeof(uint32_t);
    if(ftruncate(fd, size) < 0) {
        fprintf(stderr, "ftruncate(%s): %s\n", path, strerror(errno));
        exit(1);
    }
    uint32_t *tbl = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(tbl == MAP_FAILED) {
        fprintf(stderr, "mmap(%s): %s\n", path, strerror(errno));
        exit(1);
    }
    close(fd);
    int max_row = rows - 1;
    int min_row = 0;
    int max_col = cols - 1;
    int min_col = 0;
    int c = 1;
    int i = min_col;
    int j = min_row;
    while(1) {
        for(i = min_col; i <= max_col; i++, c++, to_visit--) {
            tbl[j * cols + i] = c;
        }
        i--;
        min_row++;
        if(!to_visit) {
            break;
        }
        for(j = min_row; j <= max_row; j++, c++, to_visit--) {
            tbl[j * cols + i] = c;
        }
        j--;
        max_col--;
        if(!to_visit) {
            break;
        }
        for(i = max_col; i >= min_col; i--, c++, to_visit--) {
            tbl[j * cols + i] = c;
        }
        i++;
        max_row--;
        if(!to_visit) {
            break;
        }
        for(j = max_row; j >= min_row; j--, c++, to_visit--) {
            tbl[j * cols + i] = c;
        }
        j++;
        min_col++;
        if(!to_visit) {
            break;
        }
    }
    munmap(tbl, size);
    return 0;
}
