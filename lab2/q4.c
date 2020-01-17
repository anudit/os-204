#include <stdio.h>

void main() {
    int n;
    printf("Enter number: ");
    scanf("%d",&n);
    int sum  = (n*(n+1))/2;
    printf("Sum of first %d numbers is %d ",n,sum);
    printf("\n");
}
// gcc q4.c && ./a.out
