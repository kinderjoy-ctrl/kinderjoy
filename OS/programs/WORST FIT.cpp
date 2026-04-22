#include <stdio.h>

int main() {
    int m, n, i, j;

    printf("Enter number of partitions: ");
    scanf("%d", &m);

    printf("Enter number of processes: ");
    scanf("%d", &n);

    int partition[m], process[n], allocated[m];

    // Initially all partitions are free
    for (i = 0; i < m; i++) {
        allocated[i] = 0;
    }

    printf("Enter partition sizes:\n");
    for (i = 0; i < m; i++) {
        scanf("%d", &partition[i]);
    }

    printf("Enter process sizes:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &process[i]);
    }

    printf("\nProcess No.\tProcess Size\tPartition No.\n");

    for (i = 0; i < n; i++) {
        int worstIndex = -1;

        for (j = 0; j < m; j++) {
            if (!allocated[j] && process[i] <= partition[j]) {
                if (worstIndex == -1 || partition[j] > partition[worstIndex]) {
                    worstIndex = j;
                }
            }
        }

        if (worstIndex != -1) {
            printf("%d\t\t%d\t\t%d\n", i + 1, process[i], worstIndex + 1);
            allocated[worstIndex] = 1;
        } else {
            printf("%d\t\t%d\t\tNot Allocated\n", i + 1, process[i]);
        }
    }

    return 0;
}
