#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

enum
{
    ARG_IPCKEY = 1,
    ARG_NPROC = 2,
    ARG_VALUE1 = 3,
    ARG_VALUE2 = 4,
    ARG_MAXVAL = 5,
    ARGS_SUCCESS = 6,
    RIGHTS = 0600
};

struct Msg
{
    long type;
    int64_t x1;
    int64_t x2;
};

int
work(int child, int msqid, int64_t maxval, int nproc)
{
    struct Msg msg;
    long msgtype = child + 1;
    while (1) {
        int n = msgrcv(msqid, &msg, sizeof(msg), msgtype, 0);
        if(n == -1) {
            if(errno == EIDRM) {
                return 0;
            }
            return 1;
        }
        int64_t x3 = msg.x1 + msg.x2;
        printf("%d %" PRId64 "\n", child, x3);
        fflush(stdout);

        if(x3 > maxval) {
            return 0;
        }

        msg.type = (x3 % (int64_t) nproc) + 1;
        msg.x1 = msg.x2;
        msg.x2 = x3;

        n = msgsnd(msqid, &msg, sizeof(msg), 0);
        if(n == -1) {
            if(errno == EIDRM) {
                return 0;
            }
            return 1;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != ARGS_SUCCESS) {
        fprintf(stderr, "args expected: KEY, nproc, value1, value2, maxval\n");
        return 1;
    }
    key_t key = strtol(argv[ARG_IPCKEY], NULL, 0);
    int nproc = strtol(argv[ARG_NPROC], NULL, 0);
    int64_t value1 = strtoimax(argv[ARG_VALUE1], NULL, 0);
    int64_t value2 = strtoimax(argv[ARG_VALUE2], NULL, 0);
    int64_t maxval = strtoimax(argv[ARG_MAXVAL], NULL, 0);

    int msqid = msgget(key, RIGHTS | IPC_CREAT | IPC_EXCL);
    if(msqid < 0) {
        fprintf(stderr, "msgget: 0x%04X %s\n", key, strerror(errno));
        if(errno == EEXIST) {
            fprintf(stderr, "msgctl: remove old message queue: 0x%04X\n", key);
            msgctl(msgget(key, RIGHTS), IPC_RMID, NULL);
        }
        return 1;
    }

    for(int i = 0; i < nproc; i++) {
        int pid = fork();
        if(pid == (pid_t) -1) {
            fprintf(stderr, "fork [%d]: %s\n", i, strerror(errno));
            msgctl(msqid, IPC_RMID, NULL);
            while(wait(NULL) != -1);
            return 1;
        }
        if(!pid) {
            exit(work(i, msqid, maxval, nproc));
        }
    }

    struct Msg msg = {1, value1, value2};
    if(msgsnd(msqid, &msg, sizeof(msg), 0) == -1) {
        fprintf(stderr, "msgsnd: %s\n", strerror(errno));
        msgctl(msqid, IPC_RMID, NULL);
        while(wait(NULL) != -1);
        return 1;
    }

    wait(NULL);
    msgctl(msqid, IPC_RMID, NULL);
    while(wait(NULL) != -1);
    return 0;
}
