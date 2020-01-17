#include <stdio.h>

void main() {
    int num, rem, rev = 0;
    printf("Enter a number : ");
    scanf("%d", &num);
    while (num != 0) {
        rem = num % 10;
        rev = rev * 10 + rem;
        num /= 10;
    }
    printf("Reversed number = %d \n", rev);
}
// gcc q2.c && ./a.out
