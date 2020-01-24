#include <stdio.h>
#define MAX_LIMIT 20

typedef struct pro {
    int processID;
    int cpuID;
    char state[10];
    int parentID;
    int totalWaitTime;
} pro;

void main () {
    int i;
    pro process[10];
    for (i = 0; i < 10; ++i) {
        char st[10];
        printf("Enter Process ID :");
        scanf("%d", &process[i].processID);
        printf("Enter CPU ID :");
        scanf("%d", &process[i].cpuID);
        printf("Enter State :");
        scanf ("%s", process[i].state);
        printf("Enter Parent ID :");
        scanf("%d", &process[i].parentID);
        printf("Enter Total Wait Time :");
        scanf("%d", &process[i].totalWaitTime);
        printf("\n");
    }

    for (i = 0; i < 10; ++i) {

        printf("Process ID %d is %d\n", i+1, process[i].processID);
        printf("CPU ID %d is %d\n", i+1, process[i].cpuID);
        printf("State %d is %s\n", i+1, process[i].state);
        printf("Parent ID %d is %d\n", i+1, process[i].parentID);
        printf("Total Wait Time %d is %d\n", i+1, process[i].totalWaitTime);
        printf("\n");
    }
}
