#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <inttypes.h>

enum { BUF_SIZE = 4 };

int
main(int argc, char **argv) 
{
    uint8_t buf[BUF_SIZE];
    int fd;
    uint32_t lbnum;

    if(argc == 1) {
        fprintf(stderr, "error: no filename is passed.\n");
        return 1;
    }

    fd = open(argv[1], O_TRUNC | O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    while(scanf("%u", &lbnum) == 1) {
        buf[0] = (lbnum >> 20) & 0x0f;
        buf[1] = (lbnum >> 12) & 0xff;
        buf[2] = (lbnum >> 8) & 0x0f;
        buf[3] = lbnum & 0xff;
        write(fd, buf, sizeof(buf));
    }

    close(fd);

    return 0;
}
