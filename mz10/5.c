#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

enum
{
    DEC = 10,
    ARGS_SUCCESS = 2
};

int
child(int fdin, int fdout, int n, int chld_n) {
    int i;
    FILE *fin = fdopen(fdin, "r");
    FILE *fout = fdopen(fdout, "w");
    while(fscanf(fin, "%d", &i) == 1 && i < n) {
        printf("%d %d\n", chld_n, i);
        fflush(stdout);
        fprintf(fout, "%d\n", i + 1);
        fflush(fout);
    }
    fclose(fin);
    fclose(fout);
    return 0;
}

int
main(int argc, char **argv)
{
    int pid1;
    int pid2;
    
    int p1[2];
    pipe(p1);
    int p2[2];
    pipe(p2);

    if(argc != ARGS_SUCCESS) {
        return 1;
    }
    
    int n = strtol(argv[1], NULL, 0);
    FILE *f = fdopen(p1[1], "w");
    fprintf(f, "%d\n", 1);

    int chld_n = 1;
    if(!(pid1 = fork())) {
        close(p1[1]);
        close(p2[0]);
        child(p1[0], p2[1], n, chld_n);
        _exit(0);
    }
    chld_n++;
    if(!(pid2 = fork())) {
        close(p1[0]);
        close(p2[1]);
        child(p2[0], p1[1], n, chld_n);
        _exit(0);
    }
    fclose(f);
    close(p1[0]);
    close(p2[0]);
    close(p2[1]);

    wait(NULL);
    wait(NULL);
    printf("Done\n");
    return 0;
}

