#include <stdio.h>
#include <stdlib.h>

enum { MAX_N = 1999 };

int
main(void)
{
    int n;
    int **table;

    if(scanf("%d", &n) != 1 && 1 < n && n <= MAX_N) {
        fprintf(stderr, "Error: couldn't read a proper N value.\n");
        return 1;
    }

    table = malloc(n * sizeof (*table));
    if(!table) {
        fprintf(stderr, "Error: couldn't allocate array.\n");
        return 1;
    }
    for(int i = 0; i < n; i++) {
        table[i] = malloc(n * sizeof(*table[i]));
        if(!table[i]) {
            fprintf(stderr, "Error: couldn't allocate array.\n");
            return 1;
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            table[(i * j) % n][i] = j;
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 1; j < n; j++) {
            printf("%d ", table[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < n; i++) {
        if(!table[i]) {
            free(table[i]);
        }
    }
    free(table);

    return 0;
}
