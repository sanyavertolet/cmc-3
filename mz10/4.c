#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void
killall(int i, pid_t *pids)
{
    for(; i > 0; i--) {
        kill(pids[i - 1], SIGKILL);
    }
    while(wait(NULL) != -1);
    free(pids);
    exit(1);
}

int
main(int argc, char **argv)
{
    if(argc == 1) {
        return 0;
    }
    pid_t *pids = malloc(sizeof(*pids) * (argc-1));
    if(!pids) {
        return 0;
    }
    int from = 0;
    for(int i = 0; i < argc - 1; i++) {
        int to[2];
        if(i < argc - 2) {
            if(pipe2(to,O_CLOEXEC) == -1) {
                killall(i, pids);
            }
        }
        pids[i] = fork();
        if(pids[i] == -1) {
            killall(i, pids);
        }
        if(!pids[i]) {
            if(i > 0) {
                dup2(from, 0);
            }
            if(i < argc - 2) {
                dup2(to[1], 1);
            }
            execlp(argv[i + 1], argv[i + 1], NULL);
            _exit(1);
        }
        if(i > 0) {
            close(from);
        }
        if(i < argc - 2) {
            close(to[1]);
            from = to[0];
        }
    }
    while(wait(NULL) != -1);
    free(pids);
    return 0;
}
