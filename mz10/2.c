#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

enum
{
    PIPE_READ = 0,
    PIPE_WRITE = 1
};

int
main(void)
{
    int p1[2];

    if(pipe(p1) == -1) {
        fprintf(stderr, "pipe(): %s\n", strerror(errno));
        return 1;
    }

    pid_t pid = fork();
    if(pid == (pid_t) -1) {
        fprintf(stderr, "fork(): %s\n", strerror(errno));
        return 1;
    }

    if(!pid) {
        close(p1[PIPE_WRITE]);
        pid = fork();
        if(pid == (pid_t) -1) {
            fprintf(stderr, "fork(): %s\n", strerror(errno));
            exit(1);
        }

        if(!pid) {
            int32_t n;
            int64_t sum = 0;
            while(read(p1[PIPE_READ], &n, sizeof(n)) == sizeof(n)) {
                sum += n;
            }
            printf("%" PRId64 "\n", sum);
            exit(0);
        } else {
            close(p1[PIPE_READ]);
            wait(NULL);
            _exit(0);
        }
    }

    close(p1[PIPE_READ]);

    int32_t x;
    int n;
    while(scanf("%" SCNd32, &x) == 1) {
        n = write(p1[PIPE_WRITE], &x, sizeof(x));
        if(n == -1) {
            fprintf(stderr, "parent %d: write error: %s\n", pid,
                    strerror(errno));
            break;
        }
    }

    close(p1[PIPE_WRITE]);

    wait(NULL);

    return 0;
}
