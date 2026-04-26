#include <stdio.h>
#define MAX_PAGES 100
#define MAX_FRAMES 10
int main() {
int reference_string[MAX_PAGES];
int frames[MAX_FRAMES];
int num_pages, num_frames;
int page_faults = 0;
int stack_top = -1; // Top of the stack (index of the most recent page in frames array)
int i, j;
// Get input from user
printf("Enter the number of frames (max %d): ", MAX_FRAMES);
scanf("%d", &num_frames);
printf("Enter the number of pages in the reference string (max %d): ", MAX_PAGES);
scanf("%d", &num_pages);
printf("Enter the reference string: ");
for (i = 0; i < num_pages; i++) {
scanf("%d", &reference_string[i]);
}
// Initialize frames to -1 (indicating empty)
for (i = 0; i < num_frames; i++) {
frames[i] = -1;
}
printf("\nPage Reference | Frames (LIFO)\n");
printf("--------------------------------\n");
for (i = 0; i < num_pages; i++) {
int current_page = reference_string[i];
int page_found = 0;
// Check if the page is already in the frames
for (j = 0; j < num_frames; j++) {
if (frames[j] == current_page) {
page_found = 1;
break;
}
}
if (!page_found) {
// Page fault occurs
page_faults++;
// If frames are not full, add the page to the next available frame (effectively pushing to stack)
if (stack_top + 1 < num_frames) {
stack_top++;
frames[stack_top] = current_page;
} else {
// If frames are full, replace the LIFO page (the one at stack_top)
frames[stack_top] = current_page;
}
}
// Print current state of frames
printf("%d\t\t|", current_page);
for (j = 0; j < num_frames; j++) {
if (frames[j] != -1) {
printf(" %d", frames[j]);
} else {
printf(" -");
}
}
printf("\n");
}
printf("\nTotal Page Faults: %d\n", page_faults);
return 0;
}
