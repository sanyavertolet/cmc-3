#include <sys/wait.h>

#include <unistd.h>
#include <stdio.h>

int
main(void)
{
    printf("1");
    int n;
    scanf("%d", &n);
    fflush(stdout);
    if(n > 1) {
        int pid = fork();
        if(!pid) {
            for(int i = 1; i < n; ++i) {
                printf(" %d", i + 1);
                fflush(stdout);
                if(i + 1 < n) {
                    int pid = fork();
                    if(pid) {
                        wait(NULL);
                        _exit(0);
                    }
                } else {
                    _exit(0);
                }
            }
        }
        wait(NULL);
    }
    printf("\n");
    return 0;
}
