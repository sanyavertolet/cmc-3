#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/sem.h>

enum
{
    ARG_NPROC = 1,
    ARG_IPCKEY = 2,
    ARG_MAXVAL = 3,
    ARGS_SUCCESS = 4,
    RIGHTS = 0600
};

int
sem_up(int semid, int n)
{
    struct sembuf op[] = {{ .sem_num = n, 1, 0 }};
    return semop(semid, op, 1);
}

int
sem_down(int semid, int n)
{
    struct sembuf op[] = {{ .sem_num = n, -1, 0 }};
    return semop(semid, op, 1);
}

int
work(int32_t chld_num, int semid, int32_t *ptr, int32_t max, int nproc)
{
    int32_t val;
    while (1) {
        if(sem_down(semid,chld_num) == -1) {
            if(errno == EIDRM) {
                return 0;
            }
            return 1;
        }
        val = ptr[0];

        printf("%" PRId32 " %" PRId32 " %"PRId32"\n", chld_num, val, ptr[1]);
        fflush(stdout);
        val++;
        if(val > max) {
            return 0;
        }

        int64_t next;
        next = val % nproc;
        next = (next * next) % nproc;
        next = (next * next) % nproc + 1;

        ptr[0] = val;
        ptr[1] = chld_num;

        sem_up(semid,next);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != ARGS_SUCCESS) {
        fprintf(stderr, "args expected: nproc, ipckey, maxval\n");
        return 1;
    }

    int nproc = strtol(argv[ARG_NPROC], NULL, 0);
    key_t key = strtol(argv[ARG_IPCKEY], NULL, 0);
    int max = strtol(argv[ARG_MAXVAL], NULL, 0);

    int semid = semget(key, nproc + 1, RIGHTS | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        fprintf(stderr, "semget: 0x%04X %s\n", key, strerror(errno));
        if(errno == EEXIST) {
            fprintf(stderr, "semctl: remove old semaphore array: 0x%04X\n", key);
            semctl(semget(key, 0, RIGHTS), 0, IPC_RMID);
        }
        return 1;
    }

    for(int i = 0 ; i < nproc + 1; ++i) {
        semctl(semid, i, SETVAL, 0);
    }

    int32_t *ptr = mmap(NULL, 2 * sizeof(*ptr), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if(!ptr) {
        fprintf(stderr, "mmap: %s\n", strerror(errno));
        semctl(semid, 0, IPC_RMID);
        return 1;
    }

    for(int i = 0; i < nproc; i++) {
        pid_t pid = fork();
        if(pid == (pid_t) -1) {
            semctl(semid, 0, IPC_RMID);
            while(wait(NULL) != -1);
            munmap(ptr, 2 * sizeof(*ptr));
            return 1;
        }
        if(!pid) {
            _exit(work(i + 1, semid, ptr, max, nproc));
        }
    }
    sem_up(semid, 1);

    wait(NULL);

    semctl(semid, 0, IPC_RMID);

    while(wait(NULL) != -1);
    munmap(ptr, 2 * sizeof(*ptr));

    return 0;
}
