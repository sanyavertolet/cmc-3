#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

enum { FNAME_LEN = 4096 };

int
main(int argc, char **argv)
{
    char suffix[] = ".exe";
    size_t suffix_len = strlen(suffix);

    if(argc == 1) {
        fprintf(stderr, "error: no path passed to program.");
        return 1;
    }

    DIR *dir = opendir(argv[1]);
    if(!dir) {
        fprintf(stderr, "error: no such folder.");
        return 1;
    }

    size_t path_len = strlen(argv[1]);
    char *full_fname = calloc(1, path_len + 1 + FNAME_LEN + 1);
    if(!full_fname) {
        fprintf(stderr, "error: no memory for full name.");
        return 1;
    }
    
    strcpy(full_fname, argv[1]);
    if(full_fname[path_len - 1] != '/') {
        full_fname[path_len] = '/';
        ++path_len;
    }
    
    struct dirent *entry;
    int counter = 0;
    while((entry = readdir(dir)) != NULL) {
        size_t name_len = strlen(entry->d_name);
        if(name_len >= suffix_len) {
            memcpy(&full_fname[path_len], entry->d_name, name_len + 1);
            struct stat buf;
            if(!stat(full_fname, &buf)) {
                if(S_ISREG(buf.st_mode)
                        && !access(full_fname, X_OK)
                        && !strcmp(entry->d_name + strlen(entry->d_name) - 4, suffix)) {
                    counter++;
                }
            }
        }
    }
 
    free(full_fname);
    
    printf("%d\n", counter);

    closedir(dir);
    return 0;
}

