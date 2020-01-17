#include <stdio.h>
#include <stdbool.h>

bool isPrime(int num) {
    int i;
    for (i = 2; i <= num - 1; i++) {
        if (num%i == 0)
        return false;
    }
    if (i == num)
        return true;
}

void main() {
   int n;
   bool result;

   printf("Enter a Number : ");
   scanf("%d",&n);

   result = isPrime(n);

   if (result == true)
      printf("%d is a prime.\n", n);
   else
      printf("%d is not a prime.\n", n);

}
