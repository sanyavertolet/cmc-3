#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/signalfd.h>

#include <string.h>
#include <errno.h>


enum
{
    ARG_FNAME = 1,
    ARGS_SUCCESS = 2,
    BYTE = 8,
    MASK = 0x80
};

int
child1(void) {
    sigset_t old_mask, new_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGUSR1);
    sigaddset(&new_mask, SIGUSR2);
    sigaddset(&new_mask, SIGIO);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);

    int sfd = signalfd(-1, &new_mask, 0);
    if(sfd == -1) {
        return 1;
    }
    while(1) {
        int ch = 0;
        for(int i = 0; i < BYTE; ++i) {
            struct signalfd_siginfo si;
            int n = read(sfd, &si, sizeof(si));
            if(n != sizeof(si)) {
                close(sfd);
                return 1;
            }
            switch(si.ssi_signo) {
                case SIGUSR1:
                    ch <<= 1;
                    kill(si.ssi_pid, SIGALRM);
                    break;
                case SIGUSR2:
                    ch <<= 1;
                    ch |= 1;
                    kill(si.ssi_pid, SIGALRM);
                    break;
                case SIGIO:
                    goto End;
                default:
                    --i;
            }
        }
        putchar(ch);
    }
End:
    close(sfd);
    return 0;
}

volatile sig_atomic_t got_sig2 = 0;
void
handler2(int signal) {
    if(signal == SIGALRM) {
        got_sig2 = 1;
    }
    return;
}

int
child2(FILE *fin, pid_t pid1) {
    sigset_t old_mask, new_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);
    sigaction(SIGALRM, &(struct sigaction) { .sa_handler = handler2 }, NULL);

    int ch;
    while((ch = fgetc(fin)) != EOF) {
        for(int i = 0; i < BYTE; ++i) {
            if(ch & MASK) {
                kill(pid1, SIGUSR2);
            } else {
                kill(pid1, SIGUSR1);
            }
            ch <<= 1;
            do {
                sigsuspend(&old_mask);
            } while(!got_sig2);
        }
    }
    kill(pid1, SIGIO);
    fclose(fin);
    return 0;
}

int
main(int argc, char **argv)
{
    if(argc != ARGS_SUCCESS) {
        fprintf(stderr, "%d args expected\n", ARGS_SUCCESS-1);
        return 1;
    }

    FILE *fin = fopen(argv[ARG_FNAME], "r");
    if(fin == NULL) {
        fprintf(stderr,"fopen(%s): %s", argv[ARG_FNAME], strerror(errno));
        return 1;
    }

    int pid1 = fork();
    if(pid1 == -1) {
        fprintf(stderr, "fork() for child 1: %s\n", strerror(errno));
        return 1;
    }
    if(!pid1) {
        exit(child1());
    }

    int pid2 = fork();
    if(pid2 == -1) {
        fprintf(stderr, "fork() for child 2: %s\n", strerror(errno));
        kill(pid1, SIGTERM);
        wait(NULL);
        return 1;
    }
    if(!pid2) {
        _exit(child2(fin, pid1));
    }

    fclose(fin);

    int status;
    pid_t pid = wait(&status);
    if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        if(pid == pid1) {
            kill(pid2, SIGTERM);
        } else if(pid == pid2) {
            kill(pid1, SIGTERM);
        }
    }
    wait(NULL);
    return 0;
}
