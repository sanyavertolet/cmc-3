#define _GNU_SOURCE
#include <fcntl.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

enum
{
    ARG_COUNT = 1,
    ARG_MOD = 2,
    ARG_PRG = 3,
    ARGS_SUCCESS = 4,
    BUF_SIZE = 1024
};

int
main(int argc, char **argv)
{
    if(argc != ARGS_SUCCESS) {
        fprintf(stderr, "%d args expexted: count, mod, program\n", ARGS_SUCCESS-1);
        return 1;
    }
    int32_t count = strtol(argv[ARG_COUNT], NULL, 0);
    int32_t modulo = strtol(argv[ARG_MOD], NULL, 0);
    int p24[2];
    if(pipe2(p24, O_CLOEXEC) == -1) {
        fprintf(stderr, "Parent: pipe ERROR: %s\n", strerror(errno));
        return 1;
    }
    pid_t pid1 = fork();
    if(pid1 == (pid_t) -1) {
        fprintf(stderr, "Parent: fork for P1 failed: %s\n", strerror(errno));
        return 1;
    }
    if(!pid1) {
        close(p24[0]);
        int p12[2];
        if(pipe2(p12, O_CLOEXEC) == -1) {
            fprintf(stderr, "Child P1 (%d): pipe ERROR: %s\n", getpid(), strerror(errno));
            _exit(1);
        }
        pid_t pid2 = fork();
        if(pid2 == (pid_t) -1) {
            fprintf(stderr, "Child P1 (%d): fork for P2 failed: %s\n", getpid(), strerror(errno));
            _exit(1);
        }
        if(!pid2) {
            dup2(p12[0], STDIN_FILENO);
            dup2(p24[1], STDOUT_FILENO);
            execlp(argv[ARG_PRG], argv[ARG_PRG], NULL);
            fprintf(stderr, "Child P2 (%d): execlp %s: ERROR %s\n", getpid(), argv[ARG_PRG], strerror(errno));
            _exit(1);
        } 
        close(p12[0]);
        close(p24[1]);
        FILE *out = fdopen(p12[1], "w");
        if(!out) {
            fprintf(stderr, "Child P1 (%d): fdopen ERROR: %s\n", getpid(), strerror(errno));
            close(p12[1]);
            wait(NULL);
            _exit(1);
        }
        for(int64_t i = 1; i <= count; i++) {
            int64_t n = (i * i) % modulo;
            fprintf(out, "%" PRId64 "\n", n);
        }
        fflush(out);
        fclose(out);
        wait(NULL);
        _exit(0);
    } 
    close(p24[1]);
    pid_t pid3 = fork();
    if(pid3 == (pid_t) -1) {
        fprintf(stderr, "Parent: fork for P3 failed: %s\n", strerror(errno));
        close(p24[0]);
        wait(NULL);
        return 1;
    }
    if(!pid3) {
        pid_t pid4 = fork();
        if(pid4 == (pid_t) -1) {
            fprintf(stderr, "Child P3 (%d): fork for P3 failed: %s\n", getpid(), strerror(errno));
            _exit(1);
        }
        if(!pid4) {
            char buf[BUF_SIZE];
            int n;
            while((n = read(p24[0], buf, sizeof(buf))) > 0) {
                for(int i = 0; i < n; ++i) {
                    if(buf[i] == ' ') {
                        putchar('\n');
                    } else {
                        putchar(buf[i]);
                    }
                }
            }
            close(p24[0]);
            fflush(stdout);
            _exit(0);
        }
        close(p24[0]);
        wait(NULL);
        _exit(0);
    }
    close(p24[0]);
    while(wait(NULL) != -1);
    printf("0\n");
    return 0;
}
