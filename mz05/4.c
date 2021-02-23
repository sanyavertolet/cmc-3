#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
parse_rwx_permissions(const char *str)
{
    if (!str) {
        return -1;
    }
    const char pattern[] = "rwxrwxrwx";
    int pattern_len = sizeof(pattern) - 1;
    int str_len = strlen(str);
    int oct = 0;
    if(str_len != pattern_len) {
        return -1;
    }
    for(int i = 0; i < pattern_len; ++i) {
        if(str[pattern_len - i - 1] == pattern[pattern_len - i - 1]) {
            oct |= (1u << i);
        } else if (str[pattern_len - i - 1] != '-') {
            return -1;
        }
    }
    return oct;
}
