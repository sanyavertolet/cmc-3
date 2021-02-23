#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum { OCT = 8 };

int
main(int argc, char **argv)
{
    const char pattern[] = "rwxrwxrwx";
    int num_of_bits = sizeof(pattern) - 1;
    unsigned oct;
    for(int i = 1; i < argc; ++i) {
        char *end_ptr;
        oct = strtol(argv[i], &end_ptr, OCT);
        for(int j = num_of_bits - 1; j >= 0; --j) {
            if((oct >> j) & 1) {
                printf("%c", pattern[num_of_bits - 1 - j]);
            } else {
                printf("-");
            }
        }
        printf("\n");
    }

    return 0;
}
