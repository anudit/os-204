#include <stdio.h>
#include <stdlib.h>

void main(){

    int tc=0;
    scanf("%d", &tc);
    while (tc>0){

        unsigned long int n, q;
        scanf("%lu", &n);
        scanf("%lu", &q);

        unsigned long int a[100000];
        for(unsigned long int i=0 ; i<n ; i++)
            scanf("%lu",&a[i]);

        while (q>0){
            unsigned long int p;
            scanf("%lu", &p);

            unsigned long int even_cnt=0, odd_cnt = 0;
            for(unsigned long int j=0 ; j<n ; j++){
                unsigned long int xor = p^a[j];
                unsigned long int cntof1 =0;
                while (xor>0){
                    cntof1 += xor^1;
                    xor >>=1;
                }
                if(cntof1 %2 == 0)
                    even_cnt++;
                else
                    odd_cnt++;
            }
            printf("%lu %lu\n", even_cnt, odd_cnt);
            q--;
        }

        tc--;
    }
}
