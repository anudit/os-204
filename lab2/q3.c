#include <stdio.h>

void fib(int n) {
    static long int i=0,j=1,k;
    if(n > 0) {
        k = i + j;
        i = j;
        j = k;
        printf("%ld ",k);
        fib(n-1);
    }
}

void main() {
    int n;
    printf("Enter the number of elements: ");
    scanf("%d",&n);
    printf("Fibonacci Series: ");
    printf("%d %d ",0,1);
    fib(n-2);
    printf("\n");
}
// gcc q3.c && ./a.out
