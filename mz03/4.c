#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <ctype.h>

enum { BUF_SIZE = 16 };

int
main(void)
{
    char buf[BUF_SIZE];
    
    uint64_t sign = 1;
    uint64_t tmp = 0;
    uint64_t sum = 0;
    int n;

    while((n = (int) read(0, buf, sizeof(buf))) > 0) {
        for(int i = 0; i < n; i++) {
            if(buf[i] == '-') {
                sign = -1;
            } else if(isspace((unsigned char) buf[i])) {
                sum += sign * tmp;
                tmp = 0;
                sign = 1;
            } else if(isdigit((unsigned char) buf[i])) {
                tmp *= 10;
                tmp += buf[i] - '0';
            }
        }
    }

    sum += sign * tmp;

    printf("%"PRId64"\n", sum);

    return 0;
    
}
