#include <sys/wait.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void
son_function(void) {
    int pid = fork();
    if(pid == -1) {
        printf("-1\n");
        exit(1);
    } else if(!pid) {
        int num;
        if(scanf("%d", &num) == 1) {
            son_function();
        } else {
            _exit(0);
        }
        int status;
        wait(&status);
        if(!WEXITSTATUS(status)) {
            printf("%d\n", num);
        }
        exit(WEXITSTATUS(status));
    }
}

int
main(void)
{
    son_function();
    while(wait(NULL) == -1);
    return 0;
}
