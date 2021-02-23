#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <fcntl.h>
#include <limits.h>

enum
{
    ARGS_SUCCESS = 2,
    BUF_SIZE = PATH_MAX + 1
};

int
exec_from_file(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if(fd == -1) {
        return 1;
    }
    char buf[BUF_SIZE];
    int bytes_read = read(fd, buf, sizeof(buf));
    close(fd);
    if(bytes_read <= 1) {
        return 1;
    }
    buf[bytes_read - 1] = '\0';
    pid_t pid = fork();
    if(pid == -1) {
        return 1;
    }
    if(!pid) {
        execlp(buf, buf, NULL);
        _exit(1);
    }
    return 0;
}

int
main(int argc, char **argv)
{
    if(argc < ARGS_SUCCESS) {
        printf("%d\n", 0);
        return 0;
    }
    int n = strtol(argv[1], NULL, 0) + ARGS_SUCCESS;
    int i;
    for(i = ARGS_SUCCESS; i < argc && i < n; i++) {
        exec_from_file(argv[i]);
    }
    int counter = 0;
    int status;
    while(wait(&status) != -1) {
        counter += (WIFEXITED(status) && !WEXITSTATUS(status));
    }
    for(; i < argc; i++) {
        exec_from_file(argv[i]);
        wait(&status);
        counter += (WIFEXITED(status) && !WEXITSTATUS(status));
    }
    printf("%d\n", counter);
    return 0;
}
