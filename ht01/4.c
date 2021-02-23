#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <dlfcn.h>
#include <limits.h>

#include <stdint.h>

typedef unsigned int stack_data_t;

enum
{
    ARG_NAME = 1,
    ARG_FUNC = 2,
    ARG_SIGN = 3,
    ARG_ARGS = 4
};

static int
stack_size(char *sig)
{
    int size;
    for(size = 0; *sig; ++sig) {
        switch(*sig) {
            case 'i': {
                size += sizeof(int);
                break;
            }
            case 's': {
                size += sizeof(char*);
                break;
            }
            case 'd': {
                size += sizeof(double);
                break;
            }
            case 'v': {
                break;
            }
            default: {
                fprintf(stderr, "invalid argument signature: %c\n", *sig);
                exit(1);
            }
        }
    }
    return size;
}

int
stack_fill(stack_data_t *stackp, char *sig, char *args[])
{
    int i;
    int n;
    int nwords = 0;

    for(i = 0; *sig; ++sig, ++i) {
        switch(*sig) {
            case 'i': {
                *((int*) stackp) = (int) strtol(args[i], NULL, 0);
                n = sizeof(int);
                break;
            }
            case 's': {
                *((char**) stackp) = args[i];
                n = sizeof(char*);
                break;
            }
            case 'd': {
                *((double*) stackp) = strtod(args[i], NULL);
                n = sizeof(double);
                break;
            }
            case 'v': {
                n = 0;
                break;
            }
            default: {
                fprintf(stderr, "invalid argument sig: %c\n", *sig);
                exit(1);
            }
        }
        stackp += n / sizeof(stack_data_t);
        nwords += n / sizeof(stack_data_t);
    }
    return nwords;
}

void
print_result(int ival, double dval, char sig)
{
    switch(sig) {
        case 'i': {
            printf("%d\n", ival);
            break;
        }
        case 's': {
            printf("%s\n", (char*) ival);
            break;
        }
        case 'v': {
            break;
        }
        case 'd': {
            printf("%.10g\n", dval);
            break;
        }
        default: {
            fprintf(stderr, "invalid return value signature: %c\n", sig);
        }
    }
}

int
main(int argc, char *argv[])
{
    if(argc < ARG_ARGS) {
        fprintf(stderr, "at least 3 arguments expected:"
                " <libname> <funcname> <dl_sign> [arguments...]\n");
        exit(1);
    }
    char *dl_name = argv[ARG_NAME];
    char *dl_func = argv[ARG_FUNC];
    char *dl_sign = argv[ARG_SIGN];

    stack_data_t *stack = 0;
    int words_in_stack = 0;

    int ssize = stack_size(&dl_sign[1]);
    if(ssize) {
        stack = malloc(ssize);
        if(!stack) {
            fprintf(stderr, "can not allocate stack: %d bytes\n", ssize);
            exit(1);
        }
        words_in_stack = stack_fill(stack, &dl_sign[1], &argv[ARG_ARGS]);
    }

    void *dlid = dlopen(dl_name, RTLD_LAZY);
    if(!dlid) {
        fprintf(stderr, "dlopen(%s): %s\n", dl_name, dlerror());
        return 1;
    }

    void (*fptr)(void) = dlsym(dlid, dl_func);

    if(!fptr) {
        fprintf(stderr, "dlsym(%s): %s\n", dl_func, dlerror());
        dlclose(dlid);
        return 1;
    }

    if(ssize) {
        stack_data_t *stackp;
        for(stackp = &stack[words_in_stack - 1]; stackp >= stack; --stackp) {
            asm ("push %0\n" : : "m" (*stackp));
        }
    }

    (*fptr)();

    int ival;
    asm ("mov %%eax, %0\n" : "=m" (ival) :);

    double dval;
    asm ("fstpl %0\n" : "=m" (dval) :);

    if(ssize) {
        asm ("add %0, %%esp\n" : : "g" (ssize));
    }

    print_result(ival, dval, *dl_sign);

    dlclose(dlid);
    return 0;
}
