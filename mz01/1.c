#include <stdio.h>

    const double x_min = 1.0, x_max = 7.0, y_min = 2.0, y_max = 5.0, eq_const = 10.0;
   
int
main(void)
{
    double x, y;
    int is_inside = 0;
    
    if(scanf("%lf%lf", &x, &y) != 2) {
        return 1;
    }

    is_inside = (x_min <= x && x <= x_max && y_min <= y && y <= y_max
            && x + y <= eq_const);
    printf("%d\n", is_inside);
    
    return 0;
}
