#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

enum
{
    ARG_NPROC = 1,
    ARGS_SUCCESS = 2,
    BUF_SIZE = 100,
    RIGHTS = 0600,
    MSG_SUCCESS = 1,
    MSG_EOF = 2,
    MSG_ALL = 0
};

struct Msg
{
    long type;
    int32_t x;
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
work(int child, int semid, int msgid, int nproc)
{
    while (1) {
        if(sem_down(semid,child) == -1) {
            break;
        }
        int32_t val;
        struct Msg msg;
        if(msgrcv(msgid, &msg, sizeof(msg), MSG_ALL, 0) < 0) {
            break;
        }
        if(msg.type == MSG_EOF) {
            return 0;
        }
        val = msg.x;
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
    if(semid < 0) {
        fprintf(stderr, "semget 0x%04X: %s\n", key, strerror(errno));
        return 1;
    }

    for(int i = 0; i < nproc; ++i) {
        semctl(semid, i, SETVAL, 0);
    }

    int msgid = msgget(key, RIGHTS | IPC_CREAT | IPC_EXCL);
    if(msgid < 0) {
        semctl(semid, 0, IPC_RMID);
        fprintf(stderr, "msgget 0x%04X: %s\n", key, strerror(errno));
        return 1;
    }

    for(int i = 0; i < nproc; i++) {
        pid_t pid = fork();
        if(pid == (pid_t) -1) {
            fprintf(stderr, "fork: %s\n", strerror(errno));
            goto Error_exit;
        }
        if(!pid) {
            exit(work(i, semid, msgid, nproc));
        }
    }
    sem_up(semid, 0);
    int x;
    while(scanf("%d", &x) == 1) {
        struct Msg msg = { MSG_SUCCESS, x };
        if(msgsnd(msgid, &msg, sizeof(msg), 0) < 0) {
            goto Error_exit;
        }
    }
    struct Msg msg = { MSG_EOF };
    if(msgsnd(msgid, &msg, sizeof(msg), 0) < 0) {
            goto Error_exit;
    }
    
    wait(NULL);
    semctl(semid, 0, IPC_RMID);
    msgctl(msgid, IPC_RMID, NULL);
    while(wait(NULL) != -1);
    return 0;


Error_exit:    
    msgctl(msgid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    while(wait(NULL) != -1);
    return 1;
}
