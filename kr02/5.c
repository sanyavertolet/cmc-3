#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

#include <errno.h>
#include <string.h>

enum
{
    ARGS_SUCCESS = 6,
    ARGS_CMD1 = 1,
    ARGS_CMD2 = 2,
    ARGS_CMD3 = 3,
    ARGS_CMD4 = 4,
    ARGS_FILE1 = 5,
    FILE2_RIGHTS = 0664
};

int
main(int argc, char **argv)
{
    if(argc != ARGS_SUCCESS) {
        return 1;
    }
    int p[2];
    if(pipe(p) == -1) {
        fprintf(stderr, "pipe: %s\n", strerror(errno));
        return 1;
    }

    pid_t pid1 = fork();
    if(!pid1) {
        close(p[0]);
        dup2(p[1], 1);
        close(p[1]);
        execlp(argv[ARGS_CMD1], argv[ARGS_CMD1], NULL);
    }
    close(p[1]);

    int p2[2];
    pipe2(p2, O_CLOEXEC);
    pid_t pid2 = fork();
    if(!pid2) {
        pid_t pid2_1 = fork();
        dup2(p[0], 0);
        close(p[0]);
        if(!pid2_1) {
            int fd = open(argv[ARGS_FILE1], O_CREAT | O_TRUNC | O_RDWR);
            if(fd >= 0) {
                dup2(fd, 1);
                close(fd);
                execlp(argv[ARGS_CMD2], argv[ARGS_CMD2], NULL);
            }
        }
        int status;
        wait(&status);
        if(WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            dup2(p2[1], 1);
            execlp(argv[ARGS_CMD3], argv[ARGS_CMD3], NULL);
        }
        exit(0);
    }
    
    pid_t pid3 = fork();
    if(!pid3) {
        dup2(p2[0], 0);
        execlp(argv[ARGS_CMD4], argv[ARGS_CMD4], NULL);
    }

    close(p[0]);
    close(p[1]);
    close(p2[0]);
    close(p2[1]);
    while(wait(NULL) != -1);
    return 0;
}

