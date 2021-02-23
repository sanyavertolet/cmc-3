#include <stdio.h>
#include <string.h>
#include <time.h>

enum 
{
    SECS_PER_DAY = 24 * 60 * 60,
    YEAR_0 = 1900,
    THURSDAY = 4,
    EACH_TWO = 2, 
    EACH_FOUR = 4,
    THIRD = 3,
    MIDDAY = 12
};

int
main(void)
{
    int year;
    if(scanf("%d", &year) != 1) {
        fprintf(stderr, "error: no argument was passed!\n");
        return -1;
    }
    
    int tm_year = year - YEAR_0;
    
    struct tm dt;
    memset(&dt, 0, sizeof(dt));
    dt.tm_hour = MIDDAY;
    dt.tm_mday = 1;
    dt.tm_mon = 0;
    dt.tm_year = tm_year;
    dt.tm_isdst = -1;

    time_t t = mktime(&dt);

    int thursday_num = 0, cur_month = -1;
    
    do {
        if(cur_month != dt.tm_mon) {
            thursday_num = 0;
            cur_month = dt.tm_mon;
        }
        if(dt.tm_wday == THURSDAY) {
            if(++thursday_num == EACH_TWO || thursday_num == EACH_FOUR) {
                if(dt.tm_mday % THIRD != 0) {
                    printf("%d %d\n", dt.tm_mon + 1, dt.tm_mday);
                }
            }
        }
        t += SECS_PER_DAY;
        gmtime_r(&t, &dt);
    } while(dt.tm_year == tm_year);

    return 0;
}
