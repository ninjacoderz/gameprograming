
#include <stdio.h>

void Swap ( int *a, int *b) {
    int m = *a;
    *a = *b;
    *b = m;
}
int main() {
    int a = 10;
    int b = 20;
    Swap(&a, &b);
    printf("A: %d\n", a);
    printf("B: %d\n", b);
}