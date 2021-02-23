#include <stdio.h>

typedef int STYPE;
typedef unsigned int UTYPE;

int
bitcount(STYPE value) {
    int count = 0;
    UTYPE uvalue = (UTYPE) value;
    
    while(uvalue) {
        count += uvalue & 1u;
        uvalue >>= 1;
    }
    return count;
}

int
main(void) {
    printf("%d", bitcount((STYPE) 5));
    return 0;
}
