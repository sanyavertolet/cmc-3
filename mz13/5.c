#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

enum
{
    ARG_FNAME = 1,
    ARGS_SUCCESS = 3
};

int
getval(FILE *fin, int32_t *val)
{
    return (fscanf(fin, "%" SCNd32, val) == 1);
}

int
putval(FILE *fout, int32_t val)
{
    if(fprintf(fout, "%" PRId32 "\n", val) < 0) {
        return 0;
    }
    if(fflush(fout) == EOF) {
        return 0;
    }
    return 1;
}

int
work(int child, FILE *fin1, FILE *fin2, FILE *fout)
{
    int32_t x1, x2;

    if(!getval(fin1, &x1)) {
        goto Eof;
    }
    if(!getval(fin2, &x2)) {
        goto Eof;
    }

    while (1) {
        if(x1 == x2) {
            if(!putval(fout,  x1)) {
                goto Eof;
            }
            if(!getval(fin1, &x1)) {
                goto Eof;
            }
            if(!getval(fin2, &x2)) {
                goto Eof;
            }
            continue;
        }
        while(x1 < x2) {
            if(!getval(fin1, &x1)) {
                goto Eof;
            }
        }
        while(x2 < x1) {
            if(!getval(fin2, &x2)) {
                goto Eof;
            }
        }
    }

Eof:
    fclose(fin1);
    fclose(fin2);
    fclose(fout);
    return 0;
}

void
killall(pid_t *pid, int n)
{
    while(--n >= 0) {
        kill(pid[n], SIGTERM);
    }
    while(wait(NULL) != -1);
    exit(1);
}

int main(int argc, char *argv[])
{
    if(argc < ARGS_SUCCESS) {
        fprintf(stderr, "at least %d args expected: fname1, ...\n", ARGS_SUCCESS-1);
        return 1;
    }

    int nproc = argc - ARG_FNAME - 1;
    pid_t *pid = calloc(nproc, sizeof(*pid));
    if(!pid) {
        fprintf(stderr, "calloc: %s\n", strerror(errno));
        return 1;
    }

    int p[2];

    if((p[0] = open(argv[ARG_FNAME+0], O_RDONLY)) == -1) {
        fprintf(stderr, "open(%s): %s\n", argv[ARG_FNAME+0], strerror(errno));
        return 1;
    }

    FILE *fin1, *fin2, *fout;

    for(int i = 0; i < nproc; i++) {
        if(!(fin1 = fdopen(p[0], "r"))) {
            fprintf(stderr, "fdopen(p[0],r): %s\n", strerror(errno));
            killall(pid,i);
        }
        if(!(fin2 = fopen(argv[ARG_FNAME+i+1], "r"))) {
            fprintf(stderr, "fopen(%s): %s\n", argv[ARG_FNAME+1], strerror(errno));
            killall(pid,i);
        }
        if(pipe(p) == -1) {
            fprintf(stderr, "pipe: %s\n", strerror(errno));
            killall(pid,i);
        }
        if(!(fout = fdopen(p[1], "w"))) {
            fprintf(stderr, "fdopen(p[1],w): %s\n", strerror(errno));
            killall(pid,i);
        }
        pid[i] = fork();
        if(pid[i] == (pid_t)-1) {
            fprintf(stderr, "fork [%d]: %s\n", i, strerror(errno));
            killall(pid,i);
        }
        if(!pid[i]) {
            close(p[0]);
            exit(work(i, fin1, fin2, fout));
        }
        fclose(fin1);
        fclose(fin2);
        fclose(fout);
    }

    if(!(fin1 = fdopen(p[0], "r"))) {
        fprintf(stderr, "fdopen(p[0],r): %s\n", strerror(errno));
        killall(pid,nproc);
    }

    int32_t val;
    while(getval(fin1, &val)) {
        putval(stdout, val);
    }
    fclose(fin1);

    while(wait(NULL) != -1);
    return 0;
}
