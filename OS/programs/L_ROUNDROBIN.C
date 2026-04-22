#include <stdio.h>
 
struct Process {
    int id, at, bt, rt, ct, tat, wt;
};
 
int main() {
    int n, quantum, time = 0, completed = 0;
    float total_wt = 0, total_tat = 0;
 
    printf("Enter number of processes: ");
    scanf("%d", &n);
 
    struct Process p[n];
 
    for(int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Enter arrival time and burst time for P%d: ", i + 1);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].rt = p[i].bt;
    }
 
    printf("Enter time quantum: ");
    scanf("%d", &quantum);
 
    int queue[100], front = 0, rear = 0;
    int visited[100] = {0};
 
    // Initial queue filling
    for(int i = 0; i < n; i++) {
        if(p[i].at <= time) {
            queue[rear++] = i;
            visited[i] = 1;
        }
    }
 
    while(completed < n) {
        if(front == rear) {
            time++;
            for(int i = 0; i < n; i++) {
                if(p[i].at <= time && !visited[i]) {
                    queue[rear++] = i;
                    visited[i] = 1;
                }
            }
            continue;
        }
 
        int i = queue[front++];
 
        if(p[i].rt > quantum) {
            time += quantum;
            p[i].rt -= quantum;
        } else {
            time += p[i].rt;
            p[i].rt = 0;
            completed++;
 
            p[i].ct = time;
            p[i].tat = p[i].ct - p[i].at;
            p[i].wt = p[i].tat - p[i].bt;
 
            total_tat += p[i].tat;
            total_wt += p[i].wt;
        }
 
        // Add newly arrived processes
        for(int j = 0; j < n; j++) {
            if(p[j].at <= time && !visited[j]) {
                queue[rear++] = j;
                visited[j] = 1;
            }
        }
 
        // Re-add current process if not finished
        if(p[i].rt > 0) {
            queue[rear++] = i;
        }
    }
 
    printf("\nID\tAT\tBT\tCT\tTAT\tWT\n");
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt,
               p[i].ct, p[i].tat, p[i].wt);
    }
 
    printf("\nAverage turnaround time: %.2f", total_tat / n);
    printf("\nAverage waiting time: %.2f\n", total_wt / n);
 
    return 0;
}
 
