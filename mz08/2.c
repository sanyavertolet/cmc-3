#include <sys/wait.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    int child_pid = fork();
    if(!child_pid) {
        int grandchild_pid = fork();
        if(!grandchild_pid) {
            printf("3 ");
            exit(0);
        }
        wait(NULL);
        printf("2 ");
        exit(0);
    }
    wait(NULL);
    printf("1\n");
    return 0;
}
