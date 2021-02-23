#define _GNU_SOURCE
#include <fcntl.h>
#define debug(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)
//#define debug(fmt, ...)

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

enum
{
    ARG_COUNT = 1,
    ARG_MOD = 2,
    ARG_PRG = 3
};

int
main(int argc, char **argv)
{
    int p12[2];
    int p24[2];

    int64_t count = strtoll(argv[ARG_COUNT], NULL, 0);
    int64_t modulo = strtoll(argv[ARG_MOD], NULL, 0);

    if(pipe2(p24, O_CLOEXEC) == -1) {
        debug("Parent: pipe ERROR: %s", strerror(errno));
        exit(1);
    }
    debug("Parent: p24: [%d,%d]", p24[0], p24[1]);

    //P0
    debug("Parent: fork for P1");
    int pid1 = fork();
    if(pid1 == (pid_t)-1) {
        debug("Parent: fork failed: %s", strerror(errno));
        exit(1);
    }

    if(!pid1) {
        //P1
        debug("Child P1 (%d): close [%d]", getpid(), p24[0]);
        close(p24[0]);
        if(pipe2(p12, O_CLOEXEC) == -1) {
            debug("Child P1 (%d): pipe ERROR: %s", getpid(), strerror(errno));
            debug("Child P1 (%d): _exit(1)", getpid());
            _exit(1);
        }
        debug("Child P1 (%d): pipe p12 [%d,%d]", getpid(), p12[0], p12[1]);
        debug("Child P1 (%d): fork for P2", getpid());
        int pid2 = fork();
        if(pid2 == (pid_t)-1) {
            debug("Child P1 (%d): fork failed: %s", getpid(), strerror(errno));
            debug("Child P1 (%d): _exit(1)", getpid());
            _exit(1);
        }
        if(!pid2) {
            //P2
            debug("Child P2 (%d): close [%d]", getpid(), p12[1]);
            close(p12[1]);
            debug("Child P2 (%d): dup2 [%d] -> 0 (STDIN_FILENO)", getpid(), p12[0]);
            dup2(p12[0], STDIN_FILENO);
            debug("Child P2 (%d): close [%d]", getpid(), p12[0]);
            close(p12[0]);
            debug("Child P2 (%d): dup2 [%d] -> 1 (STDOUT_FILENO)", getpid(), p24[1]);
            dup2(p24[1], STDOUT_FILENO);
            debug("Child P2 (%d): close [%d]", getpid(), p24[1]);
            close(p24[1]);
            debug("Child P2 (%d): execlp %s", getpid(), argv[ARG_PRG]);
            execlp(argv[ARG_PRG], argv[ARG_PRG], NULL);
            debug("Child P2 (%d): execlp %s: ERROR %s", getpid(), argv[ARG_PRG], strerror(errno));
            debug("Child P2 (%d): _exit(1)", getpid());
            _exit(1);
        } //P2
        //P1
        debug("Child P1 (%d): new child P2 (%d)", getpid(), pid2);
        debug("Child P1 (%d): close [%d]", getpid(), p12[0]);
        close(p12[0]);
        debug("Child P1 (%d): close [%d]", getpid(), p24[1]);
        close(p24[1]);
        debug("Child P1 (%d): write numbers to [%d]", getpid(), p12[1]);
        FILE *out = fdopen(p12[1], "w");
        for(int64_t i = 1; i <= count; i++) {
            int64_t n = (i * i) % modulo;
            fprintf(out, "%" PRId64 " ", n);
        }
        fflush(out);
        debug("Child P1 (%d): close [%d]", getpid(), p12[1]);
        //close(p12[1]);
        fclose(out);
        debug("Child P1 (%d): wait for P2 (%d)", getpid(), pid2);
        int status;
        waitpid(pid2, &status, 0);
        debug("Child P1 (%d): P2 (%d) exited with status %d", getpid(), pid2, WEXITSTATUS(status));
        debug("Child P1 (%d): _exit(0)", getpid());
        _exit(0);
    } //P1

    //P0
    debug("Parent: new child P1 (%d)", pid1);
    debug("Parent: close [%d]", p24[1]);
    close(p24[1]);
    debug("Parent: fork for P3");
    int pid3 = fork();
    if(!pid3) {
        //P3
        debug("Child P3 (%d): fork for P4", getpid());
        int pid4 = fork();
        if(pid4 == (pid_t)-1) {
            debug("Child P3 (%d): fork failed: %s", getpid(), strerror(errno));
            debug("Child P3 (%d): _exit(1)", getpid());
            _exit(1);
        }
        if(!pid4) {
            //P4
            debug("Child P4 (%d): read and print strings", getpid());
            int64_t n;
            char buf[1024];
            while((n = read(p24[0], buf, sizeof(buf))) > 0) {
                for(int i = 0; i < n; ++i) {
                    if(buf[i] == ' ') {
                        putchar('\n');
                    } else {
                        putchar(buf[i]);
                    }
                }
            }
            fflush(stdout);
            debug("Child P4 (%d): _exit(0)", getpid());
            _exit(0);
        }
        //P3
        debug("Child P3 (%d): new child P4 (%d)", getpid(), pid4);
        debug("Child P3 (%d): close [%d]", getpid(), p24[0]);
        close(p24[0]);
        debug("Child P3 (%d): wait for P4 (%d)", getpid(), pid4);
        int status;
        waitpid(pid4, &status, 0);
        debug("Child P3 (%d): P4 (%d) exited with status %d", getpid(), pid4, WEXITSTATUS(status));
        debug("Child P3 (%d): _exit(0)", getpid());
        _exit(0);
    }
    //P0
    debug("Parent: new child P3 (%d)", pid3);
    debug("Parent: wait for child(s)");
    pid_t pid;
    int status;
    while((pid=wait(&status)) != -1) {
        char cn;
        if(pid == pid1) {
            cn = '1';
        } else if (pid == pid3) {
            cn = '3';
        } else {
            cn = '?';
        }
        debug("Parent: child P%c (%d) exited with code %d", cn, pid, WEXITSTATUS(status));
    }
    printf("0\n");
    debug("Parent: exit(0)");
    return 0;
}
