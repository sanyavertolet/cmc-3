#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

enum { DIGIT = 10 };
    
int
main(int argc, char **argv)
{
    int64_t positive_sum = 0, negative_sum = 0;
    for(int i = 1; i < argc; ++i) {
        char *end;
        int num = strtol(argv[i], &end, DIGIT);
        if(num > 0) {
            positive_sum += num;
        } else {
            negative_sum += num;
        }
    }

    printf("%" PRId64 "\n%" PRId64 "\n", positive_sum, negative_sum);


    return 0;
}
