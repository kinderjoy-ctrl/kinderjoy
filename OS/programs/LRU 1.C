
#include <stdio.h>
#define MAX_FRAMES 10
// Function to find if a page exists in the current frames
int isPageInFrames(int frames[], int n, int page) {
int i;
for (i = 0; i < n; i++) {
if (frames[i] == page) return i;
}
return -1;
}
// Function to move a page to the "top" of the stack (most recent position)
void moveToTop(int frames[], int n, int index) {
int i;
int temp = frames[index];
// Shift all elements above the index down by one
for (i = index; i < n - 1; i++) {
frames[i] = frames[i + 1];
}
frames[n - 1] = temp;
}
int main() {
int n_pages, n_frames, page_faults = 0, i, j;
int pages[30], frames[MAX_FRAMES];
int current_frame_count = 0;
printf("Enter number of pages: ");
scanf("%d", &n_pages);
printf("Enter reference string: ");
for (i = 0; i < n_pages; i++) {
scanf("%d", &pages[i]);
}
printf("Enter number of frames: ");
scanf("%d", &n_frames);
// Initialize frames with -1
for (i = 0; i < n_frames; i++) frames[i] = -1;
printf("\nRef String | Frames\n");
printf("-----------|-----------\n");
for (i = 0; i < n_pages; i++) {
int page = pages[i];
int pos = isPageInFrames(frames, current_frame_count, page);
if (pos != -1) {
// Page Hit: Move the existing page to the top
moveToTop(frames, current_frame_count, pos);
} else {
// Page Fault
page_faults++;
if (current_frame_count < n_frames) {
// If there's space, just add to the top
frames[current_frame_count] = page;
current_frame_count++;
// Stack is full: Remove bottom (index 0) and push new to top
for (j = 0; j < n_frames - 1; j++) {
frames[j] = frames[j + 1];
}
frames[n_frames - 1] = page;
}
}
// Display current frames
printf(" %d | ", page);
for (j = 0; j < n_frames; j++) {
if (frames[j] != -1) printf("%d ", frames[j]);
else printf("- ");
}
printf("\n");
}
printf("\nTotal Page Faults: %d\n", page_faults);
return 0;
}
