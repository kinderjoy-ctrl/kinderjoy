#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, head, size, i, j, totalSeek = 0, direction;

    printf("Enter number of requests: ");
    scanf("%d", &n);

    int req[n];

    printf("Enter request sequence:\n");
    for(i = 0; i < n; i++) {
        scanf("%d", &req[i]);
    }

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter disk size: ");
    scanf("%d", &size);

    printf("Enter direction (0 for LEFT, 1 for RIGHT): ");
    scanf("%d", &direction);

    // Sort requests
    for(i = 0; i < n; i++) {
        for(j = i + 1; j < n; j++) {
            if(req[i] > req[j]) {
                int temp = req[i];
                req[i] = req[j];
                req[j] = temp;
            }
        }
    }

    int pos;

    for(i = 0; i < n; i++) {
        if(head < req[i]) {
            pos = i;
            break;
        }
    }

    printf("\nSeek Sequence: %d", head);

    // If direction is RIGHT
    if(direction == 1) {
        // Move right
        for(i = pos; i < n; i++) {
            totalSeek += abs(req[i] - head);
            head = req[i];
            printf(" -> %d", head);
        }

        // Go to end
        totalSeek += abs((size - 1) - head);
        head = size - 1;
        printf(" -> %d", head);

        // Move left
        for(i = pos - 1; i >= 0; i--) {
            totalSeek += abs(req[i] - head);
            head = req[i];
            printf(" -> %d", head);
        }
    }
    // If direction is LEFT
    else {
        // Move left
        for(i = pos - 1; i >= 0; i--) {
            totalSeek += abs(req[i] - head);
            head = req[i];
            printf(" -> %d", head);
        }

        // Go to start
        totalSeek += abs(head - 0);
        head = 0;
        printf(" -> %d", head);

        // Move right
        for(i = pos; i < n; i++) {
            totalSeek += abs(req[i] - head);
            head = req[i];
            printf(" -> %d", head);
        }
    }

    printf("\nTotal Seek Time = %d\n", totalSeek);

    return 0;
}
