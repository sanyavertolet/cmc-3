#include <stdio.h>
#include <string.h>
#include <ctype.h>

int
main(int argc, char **argv)
{
    for(int i = 0; i < argc; ++i) {
        size_t len = strlen(argv[i]);
        int is_pal = 1;
        int is_alonly = 1;
        int is_empty = 1;
        for(size_t j = 0; j < len / 2; ++j) {
            is_empty = 0;
            if(argv[i][j] != argv[i][len - j - 1]) {
                is_pal = 0;
                break;
            }
            if(!islower((unsigned char) argv[i][j])) {
                is_alonly = 0;
                break;
            }
        }
        if(!islower((unsigned char) argv[i][len / 2])) {
            is_alonly = 0;
        }
        
        if(!is_empty && is_pal && is_alonly) {
            printf("%s\n", argv[i]);
        }
    }
}
