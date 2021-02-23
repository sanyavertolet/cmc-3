#include <stdio.h>
#include <time.h>
#include <string.h>

enum
{
    SCANF_SUCCESS = 4,
    MIDDAY = 12,
    YEAR_0 = 1900,
    SECS_PER_DAY = 24 * 60 * 60,
    SUNDAY = 0,
    SATURDAY = 6,
    HALF_YEAR = 6,
};

time_t
get_next_date(struct tm dt)
{
    int day = dt.tm_mday;
    time_t t = mktime(&dt) + SECS_PER_DAY;
    for(int i = HALF_YEAR; i != 0; t += SECS_PER_DAY) {
        localtime_r(&t, &dt);
        if(dt.tm_mday == day) {
            --i;
        }
    }

    return t - SECS_PER_DAY;
}

int
main(void)
{
    int year, month, day;
    int length;

    if(scanf("%d-%d-%d %d", &year, &month, &day, &length) != SCANF_SUCCESS) {
        fprintf(stderr, "error: not enough arguments were passed.");
        return 1;
    }

    struct tm dt;
    memset(&dt, 0, sizeof(dt));
    dt.tm_hour = MIDDAY;
    dt.tm_mday = day;
    dt.tm_mon = month - 1;
    dt.tm_year = year - YEAR_0;
    dt.tm_isdst = -1;


    for(int i = 0; i < length + length; ++i) {
        struct tm tmp;
        time_t t = get_next_date(dt);
        
        localtime_r(&t, &dt);
        if(dt.tm_wday == SATURDAY) {
            t = mktime(&dt) + SECS_PER_DAY + SECS_PER_DAY;
        } else if(dt.tm_wday == SUNDAY) {
            t = mktime(&dt) + SECS_PER_DAY;
        }
        localtime_r(&t, &tmp);

        printf("%04d-%02d-%02d\n", YEAR_0 + tmp.tm_year, tmp.tm_mon + 1, tmp.tm_mday);
    }

}


