#include <sys/wait.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

enum
{
    BUF_SIZE = 9,
    DEC = 10,
    CHILDER_NUM = 3,
    OUT_SIZE = 64,
    SLEEP_STEP = 10000
};

int
main(void)
{
    for(int i = 0; i < CHILDER_NUM; i++) {
        int pid = fork();
        if(!pid) {
            int32_t num;
            char buf[BUF_SIZE];
            usleep(SLEEP_STEP * i);
            int n = read(0, buf, sizeof(buf) - 1);
            if(n == -1) {
                perror("read:");
                exit(1);
            }
            buf[n] = '\0';
            sscanf(buf, "%" SCNd32, &num);
            num *= num;
            printf("%d %" PRId32 "\n", i + 1, num);
            exit(0);
        }
    }
    
    while(wait(NULL) != -1);
    return 0;
}
