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
    ARG_OUTFILE = 1,
    ARG_MOD = 2,
    ARGS_SUCCESS = 3,
    RIGHTS = 0660
};

int
main(int argc, char *argv[])
{
    if(argc != ARGS_SUCCESS) {
        fprintf(stderr, "missing %d args", ARGS_SUCCESS - 1);
        return 1;
    }
    char *outfile = argv[ARG_OUTFILE];
    int32_t mod = (int32_t) strtol(argv[ARG_MOD], NULL, 0);
    const char *tmpdir;
    tmpdir = getenv("XDG_RUNTIME_DIR");
    if(!tmpdir) {
        tmpdir = getenv("TMPDIR");
        if(!tmpdir) {
            tmpdir = "/tmp";
        }
    }
    char tmpname[PATH_MAX + 1];
    int n = snprintf(tmpname, sizeof(tmpname), "%s/vertolet-%d.c", tmpdir, getpid());
    if(n == sizeof(tmpname) - 1) {
        fprintf(stderr, "not enough space to construct source file name\n");
        return 1;
    }
    int tmpfd = open(tmpname, O_WRONLY | O_CREAT | O_TRUNC, RIGHTS);
    if(tmpfd == -1) {
        fprintf(stderr, "open(%s): %s\n", tmpname, strerror(errno));
        return 1;
    }
    dprintf(tmpfd,
            "#include <stdio.h>\n"
            "#include <inttypes.h>\n"
            "\n"
            "int\n"
            "main(void)\n"
            "{\n"
            "  int64_t n;\n"
            "  int64_t mod = %" PRId64 ";\n"
            "  scanf(\"%%\" SCNd64, &n);\n"
            "  int64_t sum = n * (n + 1) / 2;\n"
            "  printf(\"%%\" PRId64 \"\\n\", sum %% mod);\n"
            "  return 0;\n"
            "}\n", (int64_t) mod
            );
    close(tmpfd);
    pid_t pid = fork();
    if(pid == (pid_t) -1) {
        fprintf(stderr, "fork(): %s\n", strerror(errno));
        unlink(tmpname);
        return 1;
    }
    if(!pid) {
        execlp("gcc", "gcc", "-o", outfile, tmpname, NULL);
        fprintf(stderr, "execlp(gcc ...): %s\n", strerror(errno));
        _exit(1);
    }
    int status;
    wait(&status);
    if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        fprintf(stderr, "cc: return code %d, compilation error\n", WEXITSTATUS(status));
        unlink(tmpname);
        return 1;
    }
    unlink(tmpname);
    int mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IXOTH;
    if(chmod(outfile, mode) == -1) {
        fprintf(stderr, "chmod(%s,%04o): %s\n", outfile, mode, strerror(errno));
        return 1;
    }
    return 0;
}

