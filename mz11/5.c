#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <inttypes.h>

enum
{
    COUNTER_TERM = 3,
    MIN_PRIME = 2
};

static volatile int last_prime = 0;
volatile sig_atomic_t to_terminate = 0;

void
handler(int signal)
{
    static int counter;
    if(signal == SIGTERM) {
        to_terminate = 1;
    } else if(signal == SIGINT) {
        if(counter == COUNTER_TERM) {
            to_terminate = 1;
        } else {
            printf("%d\n", last_prime);
            counter++;
            fflush(stdout);
        }
    }
}

int
is_prime(unsigned n)
{
    if(n < MIN_PRIME) {
        return 0;
    }
    for(unsigned i = MIN_PRIME; i * i <= n; i++) {
        if(n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int
main(int argc, char **argv)
{
    int low, high;
    scanf("%d %d", &low, &high);
    sigaction(SIGINT, &(struct sigaction){ .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGTERM, &(struct sigaction){ .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);

    printf("%d\n", getpid());
    fflush(stdout);
    
    for(int i = low; i < high; i++) {
        if(to_terminate) {
            return 0;
        }
        if(is_prime(i)) {
            last_prime = i;
        }
    }
    printf("-1\n");
    return 0;
}
