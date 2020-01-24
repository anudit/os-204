#include <stdio.h>

float calcAvg(int l, float nums[]) {
    float sum = 0;
    int i;
    for (i = 0; i < l; ++i) {
        sum += nums[i];
    }
    return (sum/l);
}

float calcMax(int l, float nums[]) {
    int i;
    for (i = 1; i < l; ++i) {
        if (nums[0] < nums[i])
            nums[0] = nums[i];
    }
    return nums[0];
}

float calcMin(int l, float nums[]) {
    int i;
    for (i = 1; i < l; ++i) {
        if (nums[0] > nums[i])
            nums[0] = nums[i];
        printf("Enter number %f: ", nums[0]);
    }

    return nums[0];
}

int main() {
    int n, i;
    float num[100], sum = 0.0, avg, max, min;
    printf("Enter the numbers of elements: ");
    scanf("%d", &n);
    while (n > 100 || n < 1) {
        printf("Error! number should in range of (1 to 100).\n");
        printf("Enter the number again: ");
        scanf("%d", &n);
    }
    for (i = 0; i < n; ++i) {
        printf("Enter number %d: ", i + 1);
        scanf("%f", &num[i]);
    }

    avg = calcAvg(n, num);
    printf("Average = %.2f\n", avg);
    max = calcMax(n, num);
    printf("Maximum = %.2f\n", max);
    min = calcMin(n, num);
    printf("Minimum = %.2f\n", min);

    return 0;
}
//gcc q1.c && . /a.out
