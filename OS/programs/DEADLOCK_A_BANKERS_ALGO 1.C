
#include <stdio.h>
int main() {
int n, m, i, j, k;
printf("Enter the number of processes: ");
scanf("%d", &n);
printf("Enter the number of resources: ");
scanf("%d", &m);
int allocation[n][m], max[n][m], need[n][m];
int available[m], work[m];
int finish[n], safeSeq[n];
// Input Allocation Matrix
printf("Enter the allocation matrix:\n");
for (i = 0; i < n; ++i) {
for (j = 0; j < m; ++j) {
scanf("%d", &allocation[i][j]);
}
}
// Input Max Matrix
printf("Enter the maximum matrix:\n");
for (i = 0; i < n; ++i) {
for (j = 0; j < m; ++j) {
scanf("%d", &max[i][j]);
}
}
// Input Available Resources
printf("Enter the available resources:\n");
for (i = 0; i < m; ++i) {
scanf("%d", &available[i]);
}
// Initialize finish array
for (i = 0; i < n; ++i) {
finish[i] = 0;
}
// Calculate Need Matrix
for (i = 0; i < n; ++i) {
for (j = 0; j < m; ++j) {
need[i][j] = max[i][j] - allocation[i][j];
}
}
// Print Need Matrix
printf("\nNeed Matrix (Max - Allocation):\n");
for (i = 0; i < n; ++i) {
for (j = 0; j < m; ++j) {
printf("%d ", need[i][j]);
}
printf("\n");
}
// Initialize work = available
for (i = 0; i < m; ++i) {
work[i] = available[i];
}
int count = 0;
while (count < n) {
int found = 0;
for (i = 0; i < n; ++i) {
if (finish[i] == 0) {
int flag = 1;
for (j = 0; j < m; ++j) {
if (need[i][j] > work[j]) {
flag = 0;
break;
}
}
if (flag) {
for (k = 0; k < m; ++k) {
work[k] += allocation[i][k];
}
safeSeq[count++] = i;
finish[i] = 1;
found = 1;
}
}
}
if (!found) {
printf("\nSystem is not in a safe state!\n");
return 0;
}
}
printf("\nSystem is in a safe state.\n");
// Print Safe Sequence
printf("Safe sequence: ");
for (i = 0; i < n; ++i) {
printf("P%d ", safeSeq[i]);
}
printf("\n");
return 0;

}
