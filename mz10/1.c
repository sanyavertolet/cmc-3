#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

enum { YEAR = 1900 };

int
main(void)
{
    int pipefd[2];
    
    pipe(pipefd);
    
    struct tm dt;
    time_t t;

    if(!fork()) {
        if(!fork()) {
            if(!fork()) {
                close(pipefd[0]);
                t = time(NULL);
                write(pipefd[1], &t, sizeof(t));
                write(pipefd[1], &t, sizeof(t));
                write(pipefd[1], &t, sizeof(t));
                close(pipefd[1]);
                exit(0);
            }
            wait(NULL);
            close(pipefd[1]);
            read(pipefd[0], &t, sizeof(t));
            close(pipefd[0]);
            localtime_r(&t, &dt);
            printf("D:%2d\n", dt.tm_mday);
            exit(0);
            
        }
        wait(NULL);
        close(pipefd[1]);
        read(pipefd[0], &t, sizeof(t));
        close(pipefd[0]);
        localtime_r(&t, &dt);
        printf("M:%2d\n", dt.tm_mon + 1);
        exit(0);
    }
    wait(NULL);
    close(pipefd[1]);
    read(pipefd[0], &t, sizeof(t));
    close(pipefd[0]);
    localtime_r(&t, &dt);
    printf("Y:%4d\n", dt.tm_year + YEAR);
    
    return 0;
}
