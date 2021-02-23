#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

enum
{
    ARGS_SUCCESS = 2,
    SCANF_SUCCESS = 2,
    CHLD_NUMBER = 2
};

volatile sig_atomic_t sig_usr1 = 0;
void
handler(int signal) {
    sig_usr1 = 1;
    return;
}

int
child(int chld_n, FILE *in, FILE *out, int n_max, sigset_t *old_mask) {
    int i = 0;
    pid_t other_pid;
    pid_t my_pid = getpid();
    do {
        while(!sig_usr1) {
            sigsuspend(old_mask);
        }
        sig_usr1 = 0;
        if(fscanf(in, "%d %d", &i, &other_pid) != SCANF_SUCCESS) {
            break;
        }
        if(i < n_max) {
            printf("%d %d\n", chld_n, i);
            fflush(stdout);
            ++i;
        }
        fprintf(out, "%d %d\n", i, my_pid);
        fflush(out);
        kill(other_pid, SIGUSR1);
    } while(i < n_max);
    fclose(in);
    fclose(out);
    return 0;
}

int
main(int argc, char **argv)
{
    if(argc != ARGS_SUCCESS) {
        return 1;
    }
    int pid[CHLD_NUMBER];
    int pipefd[2];
    if(pipe(pipefd) == -1) {
        return 1;
    }
    sigset_t old_mask, new_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);
    sigaction(SIGUSR1, &(struct sigaction){ .sa_handler = handler }, NULL);
    int n_max = strtol(argv[1], NULL, 0);
    FILE *in = fdopen(pipefd[0], "r");
    FILE *out = fdopen(pipefd[1], "w");
    fcntl(pipefd[0], F_SETFL, fcntl(pipefd[0], F_GETFL) | O_NONBLOCK);
    for(int i = 0; i < CHLD_NUMBER; i++) {
        if(!(pid[i] = fork())) {
            child(i + 1, in, out, n_max, &old_mask);
            _exit(0);
        }
    }
    fprintf(out, "%d %d\n", 1, pid[1]);
    fflush(out);
    kill(pid[0], SIGUSR1);
    fclose(in);
    fclose(out);
    int status;
    pid_t dead_pid = wait(&status);
    if(!WIFEXITED(status)) {
        int kill_pid;
        if(dead_pid == pid[0]) {
            kill_pid = pid[1];
        } else {
            kill_pid = pid[0];
        }
        kill(kill_pid, SIGUSR1);
    }
    wait(NULL);
    printf("Done\n");
    return 0;
}
