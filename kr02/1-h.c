#include <stdio.h>
#include <inttypes.h>

int
main(void)
{
    int summon = 1;
    int reject = 2;
    int disqualify = 3;
    int32_t x;
    while(scanf("%" SCNd32, &x) == 1) {
        int expression = ((x > 5?summon:disqualify) == disqualify)?reject:summon;
        if(expression == 1) {
            printf("summon\n");
        } else if(expression == 2) {
            printf("reject\n");
        } else {
            printf("disqualify\n");
        }
    }
    return 0;
}
