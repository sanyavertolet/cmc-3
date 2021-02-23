#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

static inline int
is_any_read(int mode)
{
    return mode & (S_IRUSR|S_IRGRP|S_IROTH);
}

static inline int
is_any_write(int mode)
{
    return mode & (S_IWUSR|S_IWGRP|S_IWOTH);
}

static inline int
is_any_exec(int mode)
{
    return mode & (S_IXUSR|S_IXGRP|S_IXOTH);
}

static int
task_is_owner(const struct stat *stb, const struct Task *task)
{
    return stb->st_uid == task->uid;
}

static int
task_is_group(const struct stat *stb, const struct Task *task)
{
    unsigned *gid = task->gids;
    for(int i = 0; i < task->gid_count; ++i, ++gid) {
        if(stb->st_gid == *gid) {
            return 1;
        }
    }
    return 0;
}

int
myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if(task->uid == 0) {
        return 1;
    }

    if(access) {
        int st_mode = stb->st_mode;
        int r = is_any_read(access);
        int w = is_any_write(access);
        int x = is_any_exec(access);

        if(task_is_owner(stb, task)) {
            if(r && !(st_mode & S_IRUSR)) {
                return 0;
            }
            if(w && !(st_mode & S_IWUSR)) {
                return 0;
            }
            if(x && !(st_mode & S_IXUSR)) {
                return 0;
            }
            return 1;
        }
        if(task_is_group(stb, task)) {
            if(r && !(st_mode & S_IRGRP)) {
                return 0;
            }
            if(w && !(st_mode & S_IWGRP)) {
                return 0;
            }
            if(x && !(st_mode & S_IXGRP)) {
                return 0;
            }
            return 1;
        }
        if(r && !(st_mode & S_IROTH)) {
            return 0;
        }
        if(w && !(st_mode & S_IWOTH)) {
            return 0;
        }
        if(x && !(st_mode & S_IXOTH)) {
            return 0;
        }
        return 1;
    }

    return 0;
}
