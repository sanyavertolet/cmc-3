#include <stdio.h>

enum { ARRAY_SIZE = 1000 };

#define __C_CODE__

#ifdef __C_CODE__
int
exchange(int *a, int n)
{
    int counter = 0;
    int *eax, *ebx, ecx, edx;

    eax = a;

    edx = n;
    edx--;
    ebx = eax;
    ebx += edx;
l:
    ecx = *eax;
    if(ecx <= *ebx)
        goto s;
    edx = *ebx;
    *eax = edx;
    *ebx = ecx;
    counter++;
s:
    eax++;
    ebx--;
    if(eax < ebx)
        goto l;

    return counter;
}
#endif
#ifndef __C_CODE__
extern __attribute__ ((fastcall)) int exchange(int *a, int n);
#endif

int
main(void)
{
    int n, counter = 0;
    int a[ARRAY_SIZE];

    scanf("%d", &n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    counter = exchange(a, n);

    printf("%d", counter);
    
    return 0;
}

