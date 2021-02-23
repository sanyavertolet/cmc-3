#include <ctype.h>

int
mystrspccmp(const char *str1, const char *str2)
{
    int difference;
    do {
        while ((isspace((unsigned char) *str1) && *++str1)
                || (isspace((unsigned char) *str2) && *++str2));
        difference = (unsigned char) *str1 - (unsigned char) *str2;
    } while (difference == 0 && *str1++ && *str2++);
    return difference;
}
