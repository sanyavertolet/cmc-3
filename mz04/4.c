#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>


int
main(int argc, char **argv)
{
    if(argc != 2) {
        fprintf(stderr, "error: you should pass one filename.");
        return 1;
    }

    int fd = open(argv[1], O_RDWR);
    long long min_num = LLONG_MAX, number, offset = 0;

    int is_empty = 1;

    for(int i = 0; read(fd, &number, sizeof(number)) == sizeof(number); ++i) {
        if(min_num > number || is_empty) {
            is_empty = 0;
            min_num = number;
            offset = i * sizeof(number);
        }
    }

    lseek(fd, offset, SEEK_SET);
    if(min_num != LLONG_MIN) {
        min_num = -min_num;
    }
    if(!is_empty) {
        write(fd, &min_num, sizeof(min_num));
    }
    close(fd);
    return 0;
}
