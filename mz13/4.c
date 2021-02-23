#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

enum
{
    ARG_NPROC = 1,
    ARGS_SUCCESS = 2,
    BUF_SIZE = 100,
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
fdtoi32(int fd, int32_t *val)
{
    char buf[BUF_SIZE];
    int i = 0;
    while(1) {
        char ch;
        int n = read(fd, &ch, sizeof(ch));
        if(n != sizeof(ch)) {
            break;
        }
        if(isspace(ch)) {
            if(i) {
                break;
            } else {
                continue;
            }
        }
        buf[i++] = ch;
    }
    if(i) {
        buf[i] = '\0';
        *val = (int32_t) strtol(buf, NULL, 0);
        return 0;
    } else {
        return -1;
    }
}

int
work(int child, int semid, int fd, int nproc)
{
    while (1) {
        if(sem_down(semid,child) == -1) {
            break;
        }
        int32_t val;
        if(fdtoi32(fd, &val) == -1) {
            return 0;
        }
        printf("%d %" PRId32 "\n", child, val);
        fflush(stdout);
        int next = (nproc + val % nproc) % nproc;
        if(sem_up(semid,next) == -1) {
            break;
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{
    if(argc != ARGS_SUCCESS) {
        fprintf(stderr, "%d args expected: nproc\n", ARGS_SUCCESS-1);
        return 1;
    }

    int nproc = strtol(argv[ARG_NPROC], NULL, 0);
    key_t key = IPC_PRIVATE;

    int semid = semget(key, nproc, RIGHTS | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        fprintf(stderr, "semget 0x%04X: %s\n", key, strerror(errno));
        return 1;
    }

    for(int i = 0; i < nproc; ++i) {
        semctl(semid, i, SETVAL, 0);
    }

    for(int i = 0; i < nproc; i++) {
        int pid = fork();
        if(pid == (pid_t) -1) {
            fprintf(stderr, "fork: %s\n", strerror(errno));
            semctl(semid, 0, IPC_RMID);
            while(wait(NULL) != -1);
            return 1;
        }
        if(!pid) {
            exit(work(i, semid, STDIN_FILENO, nproc));
        }
    }
    sem_up(semid, 0);
    
    wait(NULL);
    semctl(semid, 0, IPC_RMID);
    while(wait(NULL) != -1);
    return 0;
}
