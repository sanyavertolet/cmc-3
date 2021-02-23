#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

volatile sig_atomic_t cnt;

enum { N = 5 };

void
handler(int signal)
{
    if(cnt < N) {
        printf("%d\n", cnt++);
        fflush(stdout);
    } else {
        exit(0);
    }
}

int
main(void)
{
    sigset_t old_mask, new_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGHUP);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);
    sigaction(SIGHUP, &(struct sigaction){ .sa_handler = handler }, NULL);
    
    printf("%d\n", getpid());
    fflush(stdout);

    while(cnt < N + 1) {
        sigsuspend(&old_mask);
    }

    return 0;
}
