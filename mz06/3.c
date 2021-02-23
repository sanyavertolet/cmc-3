#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum { PATH_SIZE = 4096 };

char *
relativize_path(const char *path1, const char *path2)
{
    const char *last_slash1 = path1;
    const char *last_slash2 = path2;
    char back_str[] = "../";
 
    char *relative_path = malloc(sizeof(*relative_path) * PATH_SIZE);

    relative_path[0] = '\0';

    while(*path1 == *path2 && *path1 != '\0') {
        if(*path1 == '/') {
            last_slash1 = path1;
            last_slash2 = path2;
        }
        ++path1;
        ++path2;
    }
    
    path1 = last_slash1 + 1;
    path2 = last_slash2 + 1;
    for(; *path1; ++path1) {
        if(*path1 == '/') {
            memcpy(&relative_path[strlen(relative_path)], back_str, sizeof(back_str));
        }
    }

    memcpy(&relative_path[strlen(relative_path)], path2, strlen(path2) + 1);

    return relative_path;
}
