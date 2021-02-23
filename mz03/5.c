#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>

enum 
{ 
    BUF_SIZE = 8192,
    STATS_SIZE = 10
};

int
main(void)
{
    char buf[BUF_SIZE];
    int stats[STATS_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int fd;
    int n;

    fgets(buf, BUF_SIZE, stdin);

    printf("%s\n", buf);



    if((fd = open(buf, O_RDONLY)) != -1) {
        while((n = (int) read(fd, buf, sizeof(buf))) > 0) {
            for(int i = 0; i < n; i++) {
                if(isdigit((unsigned char) buf[i])){
                    stats[buf[i] - '0']++;
                } 
            }
        }
    }
    for(int i = 0; i <= STATS_SIZE; i++) {
        printf("%d %d\n", i, stats[i]);
    }

    return 0;
}
