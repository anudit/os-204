#include<stdio.h>

int main()
{
    int n,bt[5],wt[5],tat[5],atime[5],avwt=0,avtat=0,i,j,sum=0;
    printf("Enter total number of processes(maximum 5):");
    scanf("%d",&n);

    printf("\nEnter Process Burst Time\n");
    for(i=0;i<n;i++)
    {
        printf("P[%d]:",i+1);
        scanf("%d",&bt[i]);
    }
	printf("\nEnter Process Arrival Time\n");
    for(i=0;i<n;i++)
    {
        printf("P[%d]:",i+1);
        scanf("%d",&atime[i]);
    }

    wt[0]=0;    //waiting time for first process is 0
	sum=bt[0];
    //calculating waiting time
    for(i=1;i<n;i++)
    {

        wt[i]=0;
		//sum=
		//sum+=bt[i];
        for(j=0;j<i;j++)
            wt[i]=wt[i]+bt[j];
		wt[i]-=atime[i];
    }

    printf("\nProcess\t\tBurst Time\tWaiting Time\tTurnaround Time");

    //calculating turnaround time
    for(i=0;i<n;i++)
    {
        tat[i]=bt[i]+wt[i];
        avwt+=wt[i];
        avtat+=tat[i];
        printf("\nP[%d]\t\t%d\t\t%d\t\t%d",i+1,bt[i],wt[i],tat[i]);
    }

    avwt/=i;
    avtat/=i;
    printf("\n\nAverage Waiting Time:%d",avwt);
    printf("\nAverage Turnaround Time:%d",avtat);

    return 0;
}
