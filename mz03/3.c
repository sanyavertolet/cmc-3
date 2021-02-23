#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>

enum 
{ 
    ROUND_DIGITS = 10000,
    DIGIT = 10
};

double special_round(double value) {
    value *= ROUND_DIGITS;
    value = round(value);
    value /= ROUND_DIGITS;
    return value;
}

int
main(int argc, char **argv)
{
    char *end;
    if(argc == 1) {
        fprintf(stderr, "error: no starting value is passed.\n");
        return 1;
    }
    double value = strtod(argv[1], &end);
    
    
    for(int i = 2; i < argc; ++i) {
        double change = strtod(argv[i], &end);
        value *= 1.0 + change / (1.0 * DIGIT * DIGIT);
        value = special_round(value);
    }

    printf("%.4lf\n", value);

    return 0;
    
}
