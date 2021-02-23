#include <stdio.h>
#include <ctype.h>

enum
{ 
    SYMBOLS_AMOUNT = 64,
    REVERSE_BIT = 1u << 3,
    DROPPED_BIT = 0xff ^ (1u << 2)
};

int
ascii_to_card(int ch)
{
    if(isdigit(ch)) {
        ch = ch - '0' + 1;
    } else if(islower(ch)) {
        ch = ch - 'a' + ('9' - '0' + 1) + 1;
    } else if(isupper(ch)) {
        ch = ch - 'A' + ('9' - '0' + 1) + ('z' - 'a' + 1) + 1;
    }
    return ch;
}

int
corrupt_bits(int ch)
{
    ch ^= REVERSE_BIT;
    ch &= DROPPED_BIT;
    return ch;
}

int
card_to_ascii(int ch)
{
    static const int charset[SYMBOLS_AMOUNT] = 
    { 
        '@',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '#'
    };
    return charset[ch];
}


int
main(void)
{
    int ch;
    while((ch = getchar()) != EOF) {
        if(isalnum(ch)) {
            ch = ascii_to_card(ch);
            ch = corrupt_bits(ch);   
            ch = card_to_ascii(ch);
            putchar(ch);
        }
    }
    return 0;
}
