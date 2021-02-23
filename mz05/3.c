#include <stdio.h>
#include <inttypes.h>

enum
{
    SCANF_SUCCESS = 3
};

int
main(void)
{
    int32_t n, s;
    int w;

    if(scanf("%" SCNd32 "%" SCNd32 "%d", &n, &s, &w) != SCANF_SUCCESS) {
        fprintf(stderr, "error: not enough arguments are passed.");
        return 1;
    }

    uint32_t max_num = 1u << n;
    --max_num;

    for(uint32_t i = 0;; i += s) {
        printf("|%*" PRIo32, w, i);
        printf("|%*" PRIu32, w, i);
        
        int32_t sign = -2 * ((i >> (n - 1)) & 1) + 1;
        uint32_t unsgn = (i << (32 - n + 1)) >> (32 - n + 1);
        int32_t sgn = sign * unsgn;
        printf("|%*" PRId32 "|\n", w, sgn);
        if(max_num - i < s) {
            break;
        }
    }

    return 0;
}
