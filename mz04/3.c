#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void
dfs(int fd)
{
    struct Node buf;
    read(fd, &buf, sizeof(buf));

    if(buf.right_idx != 0) {
        lseek(fd, buf.right_idx * sizeof(buf), SEEK_SET);
        dfs(fd);
    }

    printf("%" PRId32 " ", buf.key);
    
    if(buf.left_idx != 0) {
        lseek(fd, buf.left_idx * sizeof(buf), SEEK_SET);
        dfs(fd);
    }

    return;
}

int
main(int argc, char **argv)
{
    if(argc == 1) {
        fprintf(stderr, "error: no filename passed.");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);

    dfs(fd);

    printf("\n");
    
    close(fd);
    return 0;
}
