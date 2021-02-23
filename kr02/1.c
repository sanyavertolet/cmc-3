#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <sys/wait.h>
#include <sys/stat.h>

enum
{
    ARG_CODESTR = 1,
    ARGS_SUCCESS = 2,
    RIGHTS = 0664
};

int
main(int argc, char **argv) {
    if(argc != ARGS_SUCCESS) {
        return 1;
    }
    char *tmpdir = getenv("XDG_RUNTIME_DIR");
    if(!tmpdir) {
        tmpdir = getenv("TMPDIR");
        if(!tmpdir) {
            tmpdir = "/tmp";
        }
    }
    char execname[PATH_MAX + 1];
    int n = snprintf(execname, sizeof(execname), "%s/vertolet-%d", tmpdir, getpid());
    if(n == sizeof(execname) - 1) {
        fprintf(stderr, "not enough space to construct exec file name\n");
        return 1;
    }
    char srcname[PATH_MAX + 1];
    n = snprintf(srcname, sizeof(srcname), "%s.c", execname);
    if(n == sizeof(srcname) - 1) {
        fprintf(stderr, "not enough space to construct C source file name\n");
        return 1;
    }
    int tmpfd = open(srcname, O_WRONLY | O_CREAT | O_TRUNC, RIGHTS);
    if(tmpfd == -1) {
        fprintf(stderr, "open(%s): %s\n", srcname, strerror(errno));
        return 1;
    }
    dprintf(tmpfd,
            "#include <stdio.h>\n"
            "#include <inttypes.h>\n"
            "int\n"
            "main(void)\n"
            "{\n"
            "    int summon = 1;\n"
            "    int reject = 2;\n"
            "    int disqualify = 3;\n"
            "    int32_t x;\n"
            "    while(scanf(\"%%\" SCNd32, &x) == 1) {\n"
            "        int expression = %s;\n"
            "        if(expression == 1) {\n"
            "            printf(\"summon\\n\");\n"
            "        } else if(expression == 2) {\n"
            "            printf(\"reject\\n\");\n"
            "        } else {\n"
            "            printf(\"disqualify\\n\");\n"
            "        }\n"
            "    }\n"
            "    return 0;\n"
            "}\n"
            , argv[ARG_CODESTR]
            );
    close(tmpfd);
    pid_t pid = fork();
    if(pid == (pid_t) -1) {
        fprintf(stderr, "fork(): %s\n", strerror(errno));
        unlink(srcname);
        return 1;
    }
    if(!pid) {
        execlp("gcc", "gcc", "-o", execname, srcname, NULL);
        _exit(1);
    }
    wait(NULL);
    unlink(srcname);
    pid = fork();
    if(pid == (pid_t) -1) {
        fprintf(stderr, "fork(): %s\n", strerror(errno));
        unlink(execname);
        return 1;
    }
    if(!pid) {
        execlp(execname, execname, NULL);
        _exit(1);
    }
    wait(NULL);
    unlink(execname);
    return 0;
}
