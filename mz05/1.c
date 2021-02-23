#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

enum { KIBI = 1024 };

int
main(int argc, char **argv)
{
    off_t sum_size = 0;
    for(int i = 1; i < argc; ++i) {
        struct stat file_stat;
        if(!lstat(argv[i], &file_stat)
                && S_ISREG(file_stat.st_mode)
                && file_stat.st_size % KIBI == 0
                && file_stat.st_nlink == 1) {
            sum_size += file_stat.st_size;
        }
    }

    printf("%lld\n", sum_size);

    return 0;
}
