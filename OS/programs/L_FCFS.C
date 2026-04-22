#include <stdio.h>
#include <stdlib.h>
 
struct Process {
    int id, at, bt, ct, tat, wt;
};
 
void sortProcesses(struct Process p[], int n);
 
int main() {
    int n;
    float total_wt = 0, total_tat = 0;
 
    printf("Enter Number of processes: ");
    scanf("%d", &n);
 
    struct Process p[n];
 
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Enter Arrival time and Burst Time for process %d: ", i + 1);
        scanf("%d %d", &p[i].at, &p[i].bt);
    }
 
    sortProcesses(p, n);
 
    int current_time = 0;
 
    for (int i = 0; i < n; i++) {
        if (current_time < p[i].at) {
            current_time = p[i].at;
        }
 
        p[i].ct = current_time + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
 
        current_time = p[i].ct;
 
        total_wt += p[i].wt;
        total_tat += p[i].tat;
    }
 
    // Output table
    printf("\nID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt,
               p[i].ct, p[i].tat, p[i].wt);
    }
 
    printf("\nAverage Waiting Time: %.2f", total_wt / n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
 
    return 0;
}
 
void sortProcesses(struct Process p[], int n) {
    struct Process temp;
 
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}
