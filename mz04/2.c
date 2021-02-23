#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

enum
{
    NAME_SIZE = 16,
    BASE = 10
};

int
main(int argc, char **argv)
{
    int max = INT_MIN, second_max = INT_MIN, max_exists = 0, sm_exists = 0;
    for(int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        
        char name[NAME_SIZE];
        int number;

        while(read(fd, name, sizeof(name)) > 0) {
            read(fd, &number, sizeof(number));
        
            if(!max_exists || number > max) {
                if(max_exists) {
                    second_max = max;
                    sm_exists = 1;
                }
                max = number;
                max_exists = 1;
            } else if(number > second_max && number != max) {
                second_max = number;
                sm_exists = 1;
            }
        }
        close(fd);
    }
    int r, c;
    if(sm_exists) {
        r = second_max / (BASE * BASE);
        c = second_max % (BASE * BASE);
        if(second_max < 0) {
            printf("-");
            r *= -1;
            c *= -1;
        }
        printf("%d.%.2d\n", r, c);
    }
    return 0;
}
