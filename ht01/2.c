#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

enum
{
    RR_MUL = 1103515245,
    RR_INC = 12345,
    RR_MOD = 1 << 31
};

typedef unsigned __rr_int_t;

typedef struct RandomGenerator RandomGenerator;

struct RandomOperations {
    void (*destroy)(RandomGenerator *rr);
    __rr_int_t (*next)(RandomGenerator *rr);
};

struct RandomGenerator {
    __rr_int_t seed;
    __rr_int_t mul;
    __rr_int_t inc;
    __rr_int_t mod;
    struct RandomOperations *ops;
};

static __rr_int_t
rr_next(RandomGenerator *rr)
{
    return (rr->seed = (rr->seed * rr->mul + rr->inc) % rr->mod);
}

static void
rr_destroy(RandomGenerator *rr)
{
    if(rr) {
        free(rr);
    }
}

static struct
RandomOperations rr_ops = {
    &rr_destroy,
    &rr_next
};

RandomGenerator*
random_create(int seed)
{
    RandomGenerator *rr = malloc(sizeof(*rr));
    if(rr) {
        rr->mul = RR_MUL;
        rr->inc = RR_INC;
        rr->mod = RR_MOD;
        rr->ops = &rr_ops;
        rr->seed = seed;
    }
    return rr;
}
