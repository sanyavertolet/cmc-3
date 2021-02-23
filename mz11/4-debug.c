#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

#define debug(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)

enum { ARGS_SUCCESS = 2 };

//volatile sig_atomic_t sig_usr1 = 0;
void
handler(int signal) {
    //sig_usr1 = 1;
    return;
}

int
child(int chld_n, FILE *in, FILE *out, int n_max, sigset_t *old_mask) {
    int i = 0;
    pid_t other_pid;
    pid_t my_pid = getpid();

    debug("child %d (%d): start", chld_n, my_pid);

    do {
        debug("child %d (%d): wait for SIGUSR1 in main cycle", chld_n, my_pid);
        //while(!sig_usr1) {
            sigsuspend(old_mask);
        //}
        //sig_usr1 = 0;

        debug("child %d (%d): got SIGUSR1, read number(s)", chld_n, my_pid);

        if(fscanf(in, "%d %d", &i, &other_pid) != 2 ) {
            debug("child %d (%d): fscanf(): data not ready (partner terminated ubnormally)", chld_n, my_pid);
            break;
        }

        { //--------------- DEBUG ABNORMAL TERMINATION ---------------------
            if(n_max == 10 && i > n_max/2) {
                debug("child %d (%d): DEBUG: raise SIGINT !!!!", chld_n, my_pid);
                raise(SIGINT);
            }
        } //--------------- DEBUG ABNORMAL TERMINATION ---------------------

        debug("child %d (%d): number = %d, other_pid = %d", chld_n, my_pid, i, other_pid);
        if(i < n_max) {
            printf("%d %d\n", chld_n, i);
            fflush(stdout);
            ++i;
        }

        debug("child %d (%d): send new number %d to (%d)", chld_n, my_pid, i, other_pid);
        fprintf(out, "%d %d\n", i, my_pid);
        fflush(out);

        debug("child %d (%d): send SIGUSR1 to (%d)", chld_n, my_pid, other_pid);
        kill(other_pid, SIGUSR1);
    } while(i < n_max);

    debug("child %d (%d): close in and out and return", chld_n, my_pid);
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
    int pid[2];
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

    fcntl(pipefd[0],F_SETFL,fcntl(pipefd[0],F_GETFL)|O_NONBLOCK);

    for(int i = 0; i < 2; i++) {
        debug("Parent: fork child %d", i+1);
        if(!(pid[i] = fork())) {
            child(i+1, in, out, n_max, &old_mask);
            _exit(0);
        } else {
            debug("Parent: child %d (%d) forked", i+1, pid[i]);
        }
    }

    debug("Parent: print start number %d and another pid %d to child 1 (%d)", 1, pid[0], pid[0]);
    fprintf(out, "%d %d\n", 1, pid[1]);
    fflush(out);
    debug("Parent: SIGUSR1 to child 1 (%d)", pid[0]);
    kill(pid[0], SIGUSR1);

    debug("Parent: close in/out FILEs");
    fclose(in);
    fclose(out);

    debug("Parent: wait for the child to child_terminate first...");
    int status;
    pid_t dead_pid = wait(&status);
    if(!WIFEXITED(status)) {
        int kill_pid;
        if(dead_pid == pid[0]) {
            debug("Parent: child 1 (%d) terminated ABNORMALLY", dead_pid);
            kill_pid = pid[1];
        } else {
            debug("Parent: child 2 (%d) terminated ABNORMALLY", dead_pid);
            kill_pid = pid[0];
        }
        debug("Parent: send SIGUSR1 to leaving child (%d) to drop it from sigsuspend()", kill_pid);
        kill(kill_pid, SIGUSR1);
    } else {
        debug("Parent: child (%d) terminated normally", dead_pid);
    }
    debug("Parent: wait for the another child to terminate...");
    wait(NULL);
    printf("Done\n");
    debug("Parent: exit 0");
    return 0;
}
