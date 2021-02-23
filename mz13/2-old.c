#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/sem.h>

enum
{
    ARG_COUNT  = 1,
    ARG_IPCKEY = 2,
    ARG_NPROC  = 3,
    ARG_NITER  = 4,
    ARG_SRAND  = 5,
    ARGS_MIN   = 6,
    DEC = 10,
    OPS = 3,
    RIGHTS = 0600
};

int
sem_up2(int semid, int sem1, int sem2)
{
    struct sembuf op[] = {
        {0,         1, 0},
        {sem1 + 1,  1, 0},
        {sem2 + 1,  1, 0}
    };
    return semop(semid, op, OPS);
}

int
sem_down2(int semid, int sem1, int sem2)
{
    struct sembuf op[] = {
        {0,        -1, 0},
        {sem1 + 1, -1, 0},
        {sem2 + 1, -1, 0}
    };
    return semop(semid, op, OPS);
}

void
operation(int *data, int ind1, int ind2, int value)
{
    if(ind1 != ind2) {
        int tmp1 = data[ind1] - value;
        int tmp2 = data[ind2] + value;

        data[ind1] = tmp1;
        data[ind2] = tmp2;
    }
}

int
work(int semid, int *data, int niter, int count)
{
    for(int i = 0; i < niter; ++i) {
        int ind1 = rand() % count;
        int ind2 = rand() % count;
        int val = rand() % DEC;
        if(ind1 != ind2) {
            sem_down2(semid, ind1, ind2);
            operation(data, ind1, ind2, val);
            sem_up2(semid, ind1, ind2);
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc < ARGS_MIN) {
        fprintf(stderr, "at least %d args expected: count, ipckey, n_proc, n_iter, srand[n_proc]\n",
                ARGS_MIN - 1);
        return 1;
    }

    int nproc = strtol(argv[ARG_NPROC],  NULL, 0);
    int args_success = ARGS_MIN - 1 + nproc;
    if(argc < args_success) {
        fprintf(stderr, "at least %d args expected: count, ipckey, n_proc, n_iter, srand[%d]\n",
                args_success - 1, nproc);
        return 1;
    }

    int count = strtol(argv[ARG_COUNT],  NULL, 0);
    key_t key = strtol(argv[ARG_IPCKEY], NULL, 0);
    int niter = strtol(argv[ARG_NITER],  NULL, 0);

    int semid = semget(key, count + 1, RIGHTS | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        fprintf(stderr, "semget: 0x%04X %s\n", key, strerror(errno));
        if(errno == EEXIST) {
            fprintf(stderr, "semctl: remove old semaphore array: 0x%04X\n", key);
            semctl(semget(key, 0, RIGHTS), 0, IPC_RMID);
        }
        return 1;
    }
    semctl(semid, 0, SETVAL, 2);
    for(int i = 1; i <= count; ++i) {
        semctl(semid, i, SETVAL, 1);
    }

    int *data;
    int mmap_size = count * sizeof(*data);
    data = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if(!data) {
        fprintf(stderr, "mmap: %s\n", strerror(errno));
        semctl(key, 0, IPC_RMID);
        return 1;
    }
    for(int i = 0; i < count; ++i) {
        if(scanf("%d", &data[i]) != 1) {
            fprintf(stderr, "scanf [%d]: no data\n", i);
            semctl(semid, 0, IPC_RMID);
            munmap(data, mmap_size);
            return 1;
        }
    }

    for(int i = 0; i < nproc; i++) {
        int seed = strtol(argv[ARG_SRAND + i],  NULL, 0);
        srand(seed);
        int pid = fork();
        if(pid == (pid_t) -1) {
            fprintf(stderr, "fork: %s\n", strerror(errno));
            semctl(semid, 0, IPC_RMID);
            while(wait(NULL) != -1);
            munmap(data, mmap_size);
            return 1;
        }
        if(!pid) {
            exit(work(semid, data, niter, count));
        }
    }

    while(wait(NULL) != -1);
    semctl(semid, 0, IPC_RMID);

    for(int i = 0; i < count; ++i) {
        printf("%d\n", data[i]);
    }

    munmap(data, mmap_size);
    return 0;
}
