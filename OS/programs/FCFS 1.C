#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, i, head, totalSeek = 0;

    printf("Enter number of requests: ");
    scanf("%d", &n);

    int req[n];

    printf("Enter request sequence:\n");
    for(i = 0; i < n; i++) {
        scanf("%d", &req[i]);
    }

    printf("Enter initial head position: ");
    scanf("%d", &head);

    for(i = 0; i < n; i++) {
        totalSeek += abs(req[i] - head);
        head = req[i];
    }

    printf("Total Seek Time = %d\n", totalSeek);

    return 0;
}
