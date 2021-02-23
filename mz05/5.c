#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>

enum { BYTE_SIZE = 8 };

int
main(int argc, char **argv)
{
    if(argc != 2) {
        fprintf(stderr, "error: only filename should be passed.");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    
    if(fd == -1) {
        fprintf(stderr, "error: no such file.");
        return 1;
    }

    uint16_t num;
    uint16_t min_even = UINT16_MAX;
    int minev_exists = 0;
    while(read(fd, &num, sizeof(num)) == sizeof(num)) {
        num = (num << BYTE_SIZE) | (num >> BYTE_SIZE);
        if(!(num & 1) && min_even > num) {
            minev_exists = 1;
            min_even = num;
        }
    }

    if(minev_exists) {
        printf("%" PRIu16 "\n", min_even);
    }
    
    return 0;
}
