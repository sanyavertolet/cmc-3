#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <inttypes.h>

enum
{
    N = 5,
    ADD = 0,
    MUL = 1
};

static int32_t action = ADD;

void
handler(int signal)
{
    if(signal == SIGINT) {
        action = ADD;
    } else if(signal == SIGQUIT){
        action = MUL;
    }
}

int
main(void)
{
    sigset_t old_mask, new_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGINT);
    sigaddset(&new_mask, SIGQUIT);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);
    sigaction(SIGINT, &(struct sigaction){ .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGQUIT, &(struct sigaction){ .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);

    int32_t x;
    uint32_t accum = 0;

    printf("%d\n", getpid());
    fflush(stdout);

    sigprocmask(SIG_UNBLOCK, &new_mask, &old_mask);
    while(scanf("%" SCNd32, &x) == 1) {
        if(action == ADD) {
            accum += x;
        } else if(action == MUL) {
            accum *= x;
        }
        printf("%" PRId32 "\n", accum);
        fflush(stdout);
    }

    return 0;
}
