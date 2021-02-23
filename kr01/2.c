#include <stdio.h>
#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>

enum
{
    FNAME_SIZE = 4096,
    BASE = 29
};

int
is_suitable(int ch)
{
    return (ch == '-') || ('0' <= ch && ch <= '9') || ('a' <= ch && ch <= 's');
}

int
main(void)
{
    char fname[FNAME_SIZE];
    while(scanf("%s", fname) == 1) {
        int fd;
        if((fd = open(fname, O_RDONLY)) > 0) {
            char ch;
            int n=sizeof(ch);
            while(n == sizeof(ch)) {
                int64_t sign = 1;
                uint64_t num = 0;
                int got_num = 0;
                uint64_t dx = 0;
                while((n=read(fd,&ch,sizeof(ch))) == sizeof(ch)) {
                    if(!is_suitable(ch)) {
                        break;
                    }
                    if(ch == '-') {
                        sign = -1;
                    } else if('0' <= ch && ch <= '9') {
                        dx = ch - '0';
                        if(num <= (INT64_MAX - dx) / BASE) {
                            num = num * BASE + dx;
                            got_num = 1;
                        } else {
                            got_num = -1;
                            break;
                        }
                    } else if('a' <= ch && ch <= 's') {
                        dx = ch - 'a' + '9' - '0' + 1;
                        if(num <= (INT64_MAX - dx) / BASE) {
                            num = num * BASE + dx;
                            got_num = 1;
                        } else {
                            got_num = -1;
                            break;
                        }
                    }
                }
                if(got_num == 1) {
                    printf("%" PRId64 "\n", (int64_t) (sign * num));
                    got_num = 0;
                } else if(got_num == -1) {
                    //printf("too much\n");
                    while((n=read(fd,&ch,sizeof(ch))) == sizeof(ch)) {
                        if(!is_suitable(ch)) {
                            break;
                        }
                    }
                }
            }
            close(fd);
        }
    }
}
