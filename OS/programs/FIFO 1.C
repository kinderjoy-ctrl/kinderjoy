#include <stdio.h>
int main() {
int n, m, i, j, flag, page_faults = 0, first_in_index = 0;
int reference_string[30], frames[10];
printf("Enter number of pages: ");
scanf("%d", &n);
printf("Enter the reference string: ");
for (i = 0; i < n; i++)
scanf("%d", &reference_string[i]);
printf("Enter number of frames: ");
scanf("%d", &m);
// Initialize frames with -1 to indicate they are empty
for (i = 0; i < m; i++)
frames[i] = -1;
printf("\nRef String\tFrame Contents\n");
for (i = 0; i < n; i++) {
flag = 0;
// Check for Page Hit
for (j = 0; j < m; j++) {
if (frames[j] == reference_string[i]) {
flag = 1;
break;
}
}
// Page Fault logic
if (flag == 0) {
// Replace the oldest page (at first_in_index)
frames[first_in_index] = reference_string[i];
// Move the index to the next frame (circularly)
first_in_index = (first_in_index + 1) % m;
page_faults++;
}
// Display progress
printf("%d\t\t", reference_string[i]);
for (j = 0; j < m; j++) {
if (frames[j] != -1)
printf("%d ", frames[j]);
else
printf("- ");
}
if (flag == 0) printf("(Fault)");
else printf("(Hit)");
printf("\n");
}
printf("\nTotal Page Faults: %d\n", page_faults);
return 0;
}
