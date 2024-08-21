#include <stdio.h>

int main() {
    int a[3];
    int * p1 = a;
    int *p2 = a+2;
    int p = 2;
    printf("%d\n", (p2 - p1) == p);
    return 0;
}