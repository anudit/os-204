#include <stdio.h>
int main(){
    char io;
    printf("Input here: ");
    scanf("%c",&io);
    switch (io)
    {
    case '+':
        printf("Process in new state");
        break;

    case '-':
        printf("Process in ready state");
        break;

    case '*':
        printf("Process in running state");
        break;

    case '/':
        printf("Process moved to waiting state");
        break;

    case '$':
        printf("Process waiting for CPU");
        break;

    case '#':
        printf("Process completed");
        break;

    default:
        printf("Error: Wrong Input");
        break;
    }
    printf("\n");
}
//gcc q2.c && . /a.out
