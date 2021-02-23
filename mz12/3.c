#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <inttypes.h>

enum
{
    SIG_ADD = 5,
    SIG_SUB = -4
};

static volatile int32_t accum = 0;
static volatile sig_atomic_t sig = 0;

void
handler(int signal)
{
    if(signal == SIGUSR1) {
        accum += SIG_ADD;
    } else if(signal == SIGUSR2){
        accum += SIG_SUB;
    }
    sig = signal;
}

int
main(void)
{
    sigset_t old_mask, new_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGUSR1);
    sigaddset(&new_mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);
    sigaction(SIGUSR1, &(struct sigaction){ .sa_handler = handler }, NULL);
    sigaction(SIGUSR2, &(struct sigaction){ .sa_handler = handler }, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    do {
        while(!sig) {
            sigsuspend(&old_mask);
        }
        printf("%d %" PRId32 "\n", sig, accum);
        fflush(stdout);
        sig = 0;
    } while(accum >= 0);
    return 0;
}
