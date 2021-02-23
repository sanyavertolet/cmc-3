#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

extern unsigned long long hash_function(unsigned long long);

int
main(int argc, char **argv)
{
    if(argc != 4) {
        fprintf(stderr, "error: you should pass 3 arguments");
        return 1;
    }

    int fd = open(argv[1], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

    unsigned long long h_0;
    sscanf(argv[2], "%llx", &h_0);

    long long count;
    sscanf(argv[3], "%lld", &count);
    
    lseek(fd, 0, SEEK_END);
    for(long long i = 0; i < count; ++i) {
        write(fd, &h_0, sizeof(h_0));
    }

    unsigned long long h_i = h_0;
    for(long long i = 1; i < count; ++i) {
        h_i = hash_function(h_i);
        lseek(fd, -((i + 1) * sizeof(h_i)), SEEK_END);
        write(fd, &h_i, sizeof(h_i));
    }

    close(fd);
    return 0;
}
