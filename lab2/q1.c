#include <stdio.h>
#include <string.h>

void isPal(char str[])  {
    int l = 0;
    int h = strlen(str) - 1;

    while (h > l) {
        if (str[l++] != str[h--]) {
            printf("%s is not a Palindrome\n", str);
            return;
        }
    }
    printf("%s is a Palindrome\n", str);
}

void main() {

    char name[6];
    scanf ("%[^\n]%*c", name);
    isPal(name);

}
// gcc q1.c && ./a.out
