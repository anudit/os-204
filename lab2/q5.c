#include <stdio.h>
#include <string.h>

void main() {
  char a[1000], b[1000];

  printf("Enter the string 1 : \n");
  scanf ("%[^\n]%*c", a);
  printf("Enter the string 2 : \n");
  scanf ("%[^\n]%*c", b);

  strcat(a, b);

  printf("Combined String: %s\n", a);

}
// gcc q5.c && ./a.out
