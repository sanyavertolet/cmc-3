/*
g - general effective address
m - memory effective address
r - register
i - immediate value, 0..0xffffffff
n - immediate value known at compile time.
    ("i" would allow an address known only at link time)

But there are some i386-specific ones described in the processor-specific
part of the manual and in more detail in GCC's i386.h:

q - byte-addressable register (eax, ebx, ecx, edx)
A - eax or edx
a, b, c, d, S, D - eax, ebx, ecx, edx, esi, edi respectively

I - immediate 0..31
J - immediate 0..63
K - immediate 255
L - immediate 65535
M - immediate 0..3 (shifts that can be done with lea)
N - immediate 0..255 (one-byte immediate value)
O - immediate 0..32
Operands are separated by commas. Each operand has this format:
[ [asmSymbolicName] ] constraint (cvariablename)
uint32_t Mask = 1234;
uint32_t Index;
  asm ("bsfl %[aMask], %[aIndex]"
     : [aIndex] "=r" (Index)
     : [aMask] "r" (Mask)
     : "cc");

 * к выходным операндам почему-то добавляется '=' перед буквой
 */

extern int exchange(void);

//#include <test.h>

int wrapper(int number, int *array, int(*function)(void)) {

    int result;

    __asm__      //пошла вставка
    __volatile__    //не применять оптимизацию
    //__goto__    //используются локальные метки
    (
        "call *%[function]\n\t"
    ://выходные операнды
        "=c" (result)  //из ecx результат будет скопирован в result
    ://входные операнды
    [function]  "g" (*function),
                "d" (number),  //в edx будет скопирован number
                "a" (array)  //в eax будет скопирован array
    ://разрушенные регистры
    "ebx", "cc"
    //://перечень используемых локальных меток
    );
    return result;
}

#include <stdio.h>

#define NUM_OF(a) (sizeof(a)/sizeof(a[0]))

int main(void) {

    int a1[] = { 5, 2, 3, 1 };
    int a2[] = { 1, 2, 3, 5 };
    int a3[] = { 5, 3, 2, 1 };
    int a4[] = { 5, 2, 33, 1 };

    printf("%d\n", wrapper(NUM_OF(a1), a1, exchange));
    printf("%d\n", wrapper(NUM_OF(a2), a2, exchange));
    printf("%d\n", wrapper(NUM_OF(a3), a3, exchange));
    printf("%d\n", wrapper(NUM_OF(a4), a4, exchange));

    return 0;
}
