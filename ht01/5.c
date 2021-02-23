#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

enum { ARGC_SUCCESS = 2 };

int 
cmp(const void *str1, const void *str2)
{
    return strcasecmp(*(const char**) str1, *(const char**) str2);
}

int
dfs(char *path)
{   
    int path_len = strlen(path);
    DIR *dir = opendir(path);
    if(!dir) {
        fprintf(stderr, "error: no such folder.");
        return 1;
    }
    struct dirent *entry;
    char **children = malloc(sizeof(*children));
    int mem = 1;
    int n = 0;
    while((entry = readdir(dir)) != NULL) {
        if(n == mem) {
            mem *= 2;
            children = realloc(children, mem * sizeof(*children));
        }
        if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            children[n] = malloc(sizeof(**children) * (path_len + 1 + strlen(entry->d_name) + 1));
            strcpy(children[n], path);
            children[n][path_len] = '/';
            strcpy(children[n] + path_len + 1, entry->d_name);
            struct stat buf;
            lstat(children[n], &buf);
            if(!S_ISDIR(buf.st_mode) || access(children[n], R_OK)) {
                free(children[n]);
            } else {
                n++;
            }
        }
    }
    closedir(dir);
    qsort(children, n, sizeof(*children), cmp);
    for(int i = 0; i < n; i++) {
        printf("cd %s\n", children[i] + path_len + 1);
        dfs(children[i]);
        printf("cd ..\n");
        free(children[i]);
    }
    free(children);
    return 0;
}

int
main(int argc, char **argv)
{
    if(argc != ARGC_SUCCESS) {
        fprintf(stderr, "error: one argument should be passed.\n");
        return 1;
    }
    dfs(argv[1]);
    return 0;
}

