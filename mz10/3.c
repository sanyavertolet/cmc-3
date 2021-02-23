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
    ARGS_FILE1 = 4,
    ARGS_FILE2 = 5,
    FILE2_RIGHTS = 0664
};

int
main(int argc, char **argv)
{
    if(argc != ARGS_SUCCESS) {
        fprintf(stderr, "error: 5 args should be passed\n");
        return 1;
    }
    int p[2];
    if(pipe(p) == -1) {
        fprintf(stderr, "pipe: %s\n", strerror(errno));
        return 1;
    }
    pid_t pid1 = fork();
    if(pid1 == (pid_t) - 1) {
        fprintf(stderr, "fork: %s\n", strerror(errno));
        return 1;
    }
    if(!pid1) {
        close(p[0]);
        dup2(p[1], 1);
        close(p[1]);
        pid_t pid1_1 = fork();
        if(pid1_1 == (pid_t) - 1) {
            fprintf(stderr, "inner fork: %s\n", strerror(errno));
            _exit(1);
        }
        if(!pid1_1) {
            int fd = open(argv[ARGS_FILE1], O_RDONLY);
            if(fd >= 0) {
                dup2(fd, 0);
                close(fd);
                execlp(argv[ARGS_CMD1], argv[ARGS_CMD1], NULL);
                fprintf(stderr, "%s: %s\n", argv[ARGS_CMD1], strerror(errno));
            } else {
                fprintf(stderr, "%s: %s\n", argv[ARGS_FILE1], strerror(errno));
            }
            _exit(1);
        }
        int status;
        wait(&status);
        if(!WEXITSTATUS(status)) {
            execlp(argv[ARGS_CMD2], argv[ARGS_CMD2], NULL);
            fprintf(stderr, "%s: %s\n", argv[ARGS_CMD2], strerror(errno));
            _exit(1);
        }
        _exit(0);
    }
    close(p[1]);
    pid_t pid2 = fork();
    if(pid2 == (pid_t) -1) {
        fprintf(stderr, "fork: %s\n", strerror(errno));
        wait(NULL);
        close(p[0]);
        return 1;
    } else if(!pid2) {
        int fd = open(argv[ARGS_FILE2], O_CREAT | O_APPEND | O_WRONLY, FILE2_RIGHTS);
        if(fd >= 0) {
            dup2(fd, 1);
            close(fd);
            dup2(p[0], 0);
            close(p[0]);
            execlp(argv[ARGS_CMD3], argv[ARGS_CMD3], NULL);
            fprintf(stderr, "%s: %s\n", argv[ARGS_CMD3], strerror(errno));
            _exit(1);
        } else {
            close(p[0]);
            fprintf(stderr, "%s: %s\n", argv[ARGS_FILE2], strerror(errno));
            _exit(1);
        }
    }
    close(p[0]);
    int status1, status2;
    wait(&status1);
    wait(&status2);
    return !(!WEXITSTATUS(status1) & !WEXITSTATUS(status2));
}
