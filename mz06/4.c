#include <stdio.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>

enum 
{
    YEAR_0 = 1900,
    YEAR = 1925,
    MONTH = 9,
    DAY = 7,
    MIDDAY = 12,
    SCANF_SUCCESS = 3,
    SECONDS_PER_DAY = 24 * 60 * 60,
    DAYS_PER_MONTH = 30,
    MONTH_PER_YEAR = 12
};

int
main(void)
{
    struct tm start;
    memset(&start, 0, sizeof(start));
    start.tm_hour = MIDDAY;
    start.tm_mday = DAY;
    start.tm_mon = MONTH;
    start.tm_year = YEAR - YEAR_0;
    start.tm_isdst = -1;

    unsigned year, month, day;
    while(scanf("%u %u %u", &year, &month, &day) == SCANF_SUCCESS) {
        struct tm this;
        memset(&this, 0, sizeof(this));
        this.tm_hour = MIDDAY;
        this.tm_mday = day;
        this.tm_mon = month - 1;
        this.tm_year = year - YEAR_0;
        this.tm_isdst = -1;

        uint32_t t_start = mktime(&start);

        uint32_t t_this = mktime(&this);

        uint32_t delta_t = (uint32_t)difftime(t_this, t_start);

        uint32_t new_day = delta_t / SECONDS_PER_DAY + (delta_t % SECONDS_PER_DAY != 0);
        uint32_t new_month = new_day / DAYS_PER_MONTH;
        new_day %= DAYS_PER_MONTH;
        uint32_t new_year = new_month / MONTH_PER_YEAR;
        new_month %= MONTH_PER_YEAR;

        printf("%" PRIu32 " %" PRIu32 " %" PRIu32 "\n", new_year + 1, new_month + 1, new_day + 1);
    }

    return 0;
}
