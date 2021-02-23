#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/wait.h>
#include <sys/mman.h>

enum
{
    ARG_NUMBER = 1,
    ARGS_MIN = 2,
    BASE_OUT = 4
};

int
work(int child, int *data, char *sval)
{
    int64_t val = strtoll(sval, NULL, 0);

    int sum = 0;
    if(val < 0) {
        val = -val;
    }
    while(val > 0) {
        int dig = val % BASE_OUT;
        sum += dig;
        val /= BASE_OUT;
    }
    data[child] = sum;
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc < ARGS_MIN) {
        fprintf(stderr, "at least %d args expected: count, ipckey, n_proc, n_iter, srand[n_proc]\n",
                ARGS_MIN - 1);
        return 1;
    }

    int nproc = argc - ARG_NUMBER;

    int *data;
    int mmap_size = nproc * sizeof(*data);
    data = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if(!data) {
        fprintf(stderr, "mmap: %s\n", strerror(errno));
        return 1;
    }
    for(int i = 0; i < nproc; i++) {
        int pid = fork();
        if(pid == (pid_t) -1) {
            fprintf(stderr, "fork: %s\n", strerror(errno));
            while(wait(NULL) != -1);
            munmap(data, mmap_size);
            return 1;
        }
        if(!pid) {
            exit(work(i, data, argv[ARG_NUMBER+i]));
        }
    }

    while(wait(NULL) != -1);

    for(int i = 0; i < nproc; ++i) {
        printf("%d\n", data[i]);
    }

    munmap(data, mmap_size);
    return 0;
}
