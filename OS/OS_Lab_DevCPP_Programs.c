/*
============================================================
  SIES Graduate School of Technology
  OS Lab (CSEL402) - Programs executable on Dev C++ / Windows
  Course Incharge: Ms. Pooja Phad
  Class/Sem: SE/IV  |  Lab: Lab 6

  HOW TO COMPILE (Dev C++ on Windows):
    File -> New -> Project -> Console Application -> C Project
    Paste this file, then press F9 (Compile & Run)

  PROGRAMS INCLUDED:
    Exp 5  - FCFS (Non-Preemptive) Scheduling
    Exp 6  - Round Robin (Preemptive) Scheduling
    Exp 9  - Producer-Consumer using Semaphore (simulated)
    Exp 10 - Banker's Algorithm (Deadlock Avoidance)
    Exp 11 - MFT Memory Management
    Exp 12 - Best Fit Dynamic Partitioning
    Exp 13 - First Fit Dynamic Partitioning
    Exp 14 - Worst Fit Dynamic Partitioning
    Exp 15 - FIFO Page Replacement
    Exp 16 - LIFO Page Replacement
    Exp 17 - LRU Page Replacement
    Exp 18 - Disk Scheduling (FCFS, SCAN, C-SCAN)

  NOTE: Linux-specific experiments (1,2,3,4,7,8) require
        fork(), shell scripts etc. and cannot run on Dev C++.
============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

/* Helper: integer absolute value */
int iabs(int x) { return x < 0 ? -x : x; }

/* Helper: swap two ints */
void swap_int(int *a, int *b) { int t = *a; *a = *b; *b = t; }

/* Helper: print frame slot ("-" for empty) */
void print_frame(int val) {
    if (val == -1) printf("- ");
    else           printf("%d ", val);
}

/* ============================================================
   EXPERIMENT 5: FCFS (Non-Preemptive) CPU Scheduling
   ============================================================ */
void fcfs_scheduling() {
    int n, i, j;
    int pid[20], bt[20], at[20], wt[20], tat[20], ct[20];
    float total_wt = 0, total_tat = 0;

    printf("\n========== EXP 5: FCFS Scheduling ==========\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter Arrival Time and Burst Time for each process:\n");
    for (i = 0; i < n; i++) {
        pid[i] = i + 1;
        printf("P%d Arrival Time: ", pid[i]); scanf("%d", &at[i]);
        printf("P%d Burst Time  : ", pid[i]); scanf("%d", &bt[i]);
    }

    /* Sort by arrival time */
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (at[j] > at[j + 1]) {
                swap_int(&at[j],  &at[j + 1]);
                swap_int(&bt[j],  &bt[j + 1]);
                swap_int(&pid[j], &pid[j + 1]);
            }

    ct[0] = at[0] + bt[0];
    for (i = 1; i < n; i++)
        ct[i] = (ct[i - 1] > at[i] ? ct[i - 1] : at[i]) + bt[i];

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        tat[i]     = ct[i] - at[i];
        wt[i]      = tat[i] - bt[i];
        total_wt  += wt[i];
        total_tat += tat[i];
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               pid[i], at[i], bt[i], ct[i], tat[i], wt[i]);
    }
    printf("Average Waiting Time    : %.2f\n", total_wt  / n);
    printf("Average Turnaround Time : %.2f\n", total_tat / n);
}

/* ============================================================
   EXPERIMENT 6: Round Robin (Preemptive) Scheduling
   ============================================================ */
void round_robin_scheduling() {
    int n, tq, i, time = 0, done = 0;
    int bt[20], at[20], rem[20], ct[20], wt[20], tat[20];
    float total_wt = 0, total_tat = 0;

    printf("\n========== EXP 6: Round Robin Scheduling ==========\n");
    printf("Enter number of processes: "); scanf("%d", &n);
    printf("Enter Time Quantum: ");        scanf("%d", &tq);

    printf("Enter Arrival Time and Burst Time:\n");
    for (i = 0; i < n; i++) {
        printf("P%d Arrival Time: ", i + 1); scanf("%d", &at[i]);
        printf("P%d Burst Time  : ", i + 1); scanf("%d", &bt[i]);
        rem[i] = bt[i];
        ct[i]  = 0;
    }

    while (done < n) {
        int any = 0;
        for (i = 0; i < n; i++) {
            if (rem[i] > 0 && at[i] <= time) {
                any = 1;
                if (rem[i] <= tq) {
                    time  += rem[i];
                    rem[i] = 0;
                    ct[i]  = time;
                    done++;
                } else {
                    rem[i] -= tq;
                    time   += tq;
                }
            }
        }
        if (!any) time++;
    }

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        tat[i]     = ct[i] - at[i];
        wt[i]      = tat[i] - bt[i];
        total_wt  += wt[i];
        total_tat += tat[i];
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               i + 1, at[i], bt[i], ct[i], tat[i], wt[i]);
    }
    printf("Average Waiting Time    : %.2f\n", total_wt  / n);
    printf("Average Turnaround Time : %.2f\n", total_tat / n);
}

/* ============================================================
   EXPERIMENT 9: Producer-Consumer using Semaphore (Simulated)
   ============================================================ */
void producer_consumer() {
    int buffer_size, buffer[20];
    int in = 0, out = 0;
    int empty_sem, full_sem, mutex = 1;
    int choice, item;

    printf("\n========== EXP 9: Producer-Consumer (Semaphore Simulation) ==========\n");
    printf("Enter buffer size: "); scanf("%d", &buffer_size);

    empty_sem = buffer_size;
    full_sem  = 0;

    do {
        printf("\n1. Produce\n2. Consume\n3. Display Buffer\n0. Exit\nChoice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (empty_sem == 0) {
                    printf("Buffer FULL! Producer must wait.\n");
                } else {
                    printf("Enter item to produce: "); scanf("%d", &item);
                    empty_sem--;
                    mutex = 0;
                    buffer[in] = item;
                    in = (in + 1) % buffer_size;
                    mutex = 1;
                    full_sem++;
                    printf("Produced: %d | Empty=%d Full=%d\n", item, empty_sem, full_sem);
                }
                break;

            case 2:
                if (full_sem == 0) {
                    printf("Buffer EMPTY! Consumer must wait.\n");
                } else {
                    full_sem--;
                    mutex = 0;
                    item = buffer[out];
                    out  = (out + 1) % buffer_size;
                    mutex = 1;
                    empty_sem++;
                    printf("Consumed: %d | Empty=%d Full=%d\n", item, empty_sem, full_sem);
                }
                break;

            case 3: {
                int k;
                printf("Buffer contents: ");
                for (k = out; k != in; k = (k + 1) % buffer_size)
                    printf("%d ", buffer[k]);
                printf("\n");
                break;
            }
        }
    } while (choice != 0);
}

/* ============================================================
   EXPERIMENT 10: Banker's Algorithm (Deadlock Avoidance)
   ============================================================ */
void bankers_algorithm() {
    int n, m, i, j;
    int alloc[10][10], maxm[10][10], avail[10], need[10][10];

    printf("\n========== EXP 10: Banker's Algorithm ==========\n");
    printf("Enter number of processes: ");      scanf("%d", &n);
    printf("Enter number of resource types: "); scanf("%d", &m);

    printf("Enter Allocation Matrix (%d x %d):\n", n, m);
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++) scanf("%d", &alloc[i][j]);

    printf("Enter Max Matrix (%d x %d):\n", n, m);
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++) scanf("%d", &maxm[i][j]);

    printf("Enter Available Resources (%d values):\n", m);
    for (j = 0; j < m; j++) scanf("%d", &avail[j]);

    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            need[i][j] = maxm[i][j] - alloc[i][j];

    int finish[10] = {0}, safeSeq[10], work[10], count = 0;
    for (j = 0; j < m; j++) work[j] = avail[j];

    while (count < n) {
        int found = 0;
        for (i = 0; i < n; i++) {
            if (!finish[i]) {
                int possible = 1;
                for (j = 0; j < m; j++)
                    if (need[i][j] > work[j]) { possible = 0; break; }
                if (possible) {
                    for (j = 0; j < m; j++) work[j] += alloc[i][j];
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) break;
    }

    if (count == n) {
        printf("\nSystem is in SAFE STATE.\nSafe Sequence: ");
        for (i = 0; i < n; i++)
            printf("P%d%s", safeSeq[i], i < n - 1 ? " -> " : "\n");
    } else {
        printf("\nSystem is in UNSAFE STATE! Deadlock may occur.\n");
    }

    char ch;
    printf("\nCheck resource request? (y/n): ");
    scanf(" %c", &ch);
    if (ch == 'y' || ch == 'Y') {
        int pid_req, req[10], valid = 1;
        printf("Enter process number (0-indexed): "); scanf("%d", &pid_req);
        printf("Enter request vector (%d values): ", m);
        for (j = 0; j < m; j++) scanf("%d", &req[j]);
        for (j = 0; j < m; j++)
            if (req[j] > need[pid_req][j] || req[j] > avail[j])
                { valid = 0; break; }
        if (valid) {
            for (j = 0; j < m; j++) {
                avail[j]          -= req[j];
                alloc[pid_req][j] += req[j];
                need[pid_req][j]  -= req[j];
            }
            printf("Request GRANTED. System remains safe.\n");
        } else {
            printf("Request DENIED. Would lead to unsafe state.\n");
        }
    }
}

/* ============================================================
   EXPERIMENT 11: MFT Memory Management
   ============================================================ */
void mft_memory() {
    int total, n, os_size, jobs_n, i, j;
    int job_size[20], occupied[20];

    printf("\n========== EXP 11: MFT Memory Management ==========\n");
    printf("Enter total memory size (KB): "); scanf("%d", &total);
    printf("Enter OS size (KB): ");           scanf("%d", &os_size);

    int user_mem = total - os_size;
    printf("Enter number of fixed partitions: "); scanf("%d", &n);

    int part_size = user_mem / n;
    printf("\nEach partition size: %d KB\n", part_size);

    printf("Enter number of jobs: "); scanf("%d", &jobs_n);
    for (i = 0; i < jobs_n; i++) {
        printf("Job %d size (KB): ", i + 1); scanf("%d", &job_size[i]);
        occupied[i] = 0;
    }

    printf("\nPartition\tSize\tJob\t\tInternal Fragmentation\n");
    for (i = 0; i < n; i++) {
        int alloc_done = 0;
        for (j = 0; j < jobs_n; j++) {
            if (!occupied[j] && job_size[j] <= part_size) {
                printf("%d\t\t%d\tJob %d (%dKB)\t%d KB\n",
                       i+1, part_size, j+1, job_size[j], part_size - job_size[j]);
                occupied[j] = 1;
                alloc_done  = 1;
                break;
            }
        }
        if (!alloc_done)
            printf("%d\t\t%d\tEmpty\t\t%d KB\n", i+1, part_size, part_size);
    }
}

/* Helper: print allocation result table */
void print_alloc_table(int m, int *process, int *alloc, int *block_copy) {
    int i;
    printf("\nProcess\tSize\tBlock Alloc\tBlock Size\tFragmentation\n");
    for (i = 0; i < m; i++) {
        printf("%d\t%d\t", i + 1, process[i]);
        if (alloc[i] != -1)
            printf("%d\t\t%d\t\t%d\n",
                   alloc[i]+1, block_copy[alloc[i]],
                   block_copy[alloc[i]] - process[i]);
        else
            printf("Not Allocated\n");
    }
}

/* ============================================================
   EXPERIMENT 12: Best Fit
   ============================================================ */
void best_fit() {
    int n, m, i, j;
    int block[20], process[20], block_copy[20], alloc[20];

    printf("\n========== EXP 12: Best Fit Memory Allocation ==========\n");
    printf("Enter number of memory blocks: "); scanf("%d", &n);
    printf("Enter number of processes: ");      scanf("%d", &m);

    printf("Enter block sizes:\n");
    for (i = 0; i < n; i++) { scanf("%d", &block[i]); block_copy[i] = block[i]; }
    printf("Enter process sizes:\n");
    for (i = 0; i < m; i++) scanf("%d", &process[i]);
    for (i = 0; i < m; i++) alloc[i] = -1;

    for (i = 0; i < m; i++) {
        int best = -1;
        for (j = 0; j < n; j++)
            if (block[j] >= process[i])
                if (best == -1 || block[j] < block[best]) best = j;
        if (best != -1) { alloc[i] = best; block[best] -= process[i]; }
    }
    print_alloc_table(m, process, alloc, block_copy);
}

/* ============================================================
   EXPERIMENT 13: First Fit
   ============================================================ */
void first_fit() {
    int n, m, i, j;
    int block[20], process[20], block_copy[20], alloc[20];

    printf("\n========== EXP 13: First Fit Memory Allocation ==========\n");
    printf("Enter number of memory blocks: "); scanf("%d", &n);
    printf("Enter number of processes: ");      scanf("%d", &m);

    printf("Enter block sizes:\n");
    for (i = 0; i < n; i++) { scanf("%d", &block[i]); block_copy[i] = block[i]; }
    printf("Enter process sizes:\n");
    for (i = 0; i < m; i++) scanf("%d", &process[i]);
    for (i = 0; i < m; i++) alloc[i] = -1;

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            if (block[j] >= process[i]) {
                alloc[i] = j;
                block[j] -= process[i];
                break;
            }
    print_alloc_table(m, process, alloc, block_copy);
}

/* ============================================================
   EXPERIMENT 14: Worst Fit
   ============================================================ */
void worst_fit() {
    int n, m, i, j;
    int block[20], process[20], block_copy[20], alloc[20];

    printf("\n========== EXP 14: Worst Fit Memory Allocation ==========\n");
    printf("Enter number of memory blocks: "); scanf("%d", &n);
    printf("Enter number of processes: ");      scanf("%d", &m);

    printf("Enter block sizes:\n");
    for (i = 0; i < n; i++) { scanf("%d", &block[i]); block_copy[i] = block[i]; }
    printf("Enter process sizes:\n");
    for (i = 0; i < m; i++) scanf("%d", &process[i]);
    for (i = 0; i < m; i++) alloc[i] = -1;

    for (i = 0; i < m; i++) {
        int worst = -1;
        for (j = 0; j < n; j++)
            if (block[j] >= process[i])
                if (worst == -1 || block[j] > block[worst]) worst = j;
        if (worst != -1) { alloc[i] = worst; block[worst] -= process[i]; }
    }
    print_alloc_table(m, process, alloc, block_copy);
}

/* ============================================================
   EXPERIMENT 15: FIFO Page Replacement
   ============================================================ */
void fifo_page_replacement() {
    int frames, n, i, j;
    int pages[50], frame[10], pointer = 0, faults = 0;

    printf("\n========== EXP 15: FIFO Page Replacement ==========\n");
    printf("Enter number of frames: ");           scanf("%d", &frames);
    printf("Enter number of page references: ");  scanf("%d", &n);
    printf("Enter page reference string:\n");
    for (i = 0; i < n; i++) scanf("%d", &pages[i]);
    for (i = 0; i < frames; i++) frame[i] = -1;

    printf("\nPage\t|\tFrames\t\t\t|Fault\n");
    printf("----\t|\t-------\t\t\t|------\n");
    for (i = 0; i < n; i++) {
        int found = 0;
        for (j = 0; j < frames; j++)
            if (frame[j] == pages[i]) { found = 1; break; }

        printf("%d\t|\t", pages[i]);
        if (!found) {
            frame[pointer] = pages[i];
            pointer = (pointer + 1) % frames;
            faults++;
            for (j = 0; j < frames; j++) print_frame(frame[j]);
            printf("\t\t|FAULT\n");
        } else {
            for (j = 0; j < frames; j++) print_frame(frame[j]);
            printf("\t\t|No Fault\n");
        }
    }
    printf("\nTotal Page Faults : %d\n", faults);
    printf("Hit Ratio         : %.2f\n", (float)(n - faults) / n);
}

/* ============================================================
   EXPERIMENT 16: LIFO Page Replacement
   ============================================================ */
void lifo_page_replacement() {
    int frames, n, i, j;
    int pages[50], frame[10], top = 0, faults = 0;

    printf("\n========== EXP 16: LIFO Page Replacement ==========\n");
    printf("Enter number of frames: ");           scanf("%d", &frames);
    printf("Enter number of page references: ");  scanf("%d", &n);
    printf("Enter page reference string:\n");
    for (i = 0; i < n; i++) scanf("%d", &pages[i]);
    for (i = 0; i < frames; i++) frame[i] = -1;

    printf("\nPage\t|\tFrames\t\t\t|Fault\n");
    printf("----\t|\t-------\t\t\t|------\n");
    for (i = 0; i < n; i++) {
        int found = 0;
        for (j = 0; j < frames; j++)
            if (frame[j] == pages[i]) { found = 1; break; }

        printf("%d\t|\t", pages[i]);
        if (!found) {
            int placed = 0;
            for (j = 0; j < frames; j++)
                if (frame[j] == -1) { frame[j] = pages[i]; top = j; placed = 1; break; }
            if (!placed) frame[top] = pages[i];
            faults++;
            for (j = 0; j < frames; j++) print_frame(frame[j]);
            printf("\t\t|FAULT\n");
        } else {
            for (j = 0; j < frames; j++) print_frame(frame[j]);
            printf("\t\t|No Fault\n");
        }
    }
    printf("\nTotal Page Faults : %d\n", faults);
    printf("Hit Ratio         : %.2f\n", (float)(n - faults) / n);
}

/* ============================================================
   EXPERIMENT 17: LRU Page Replacement
   ============================================================ */
void lru_page_replacement() {
    int frames, n, i, j;
    int pages[50], frame[10], last_used[10], faults = 0;

    printf("\n========== EXP 17: LRU Page Replacement ==========\n");
    printf("Enter number of frames: ");           scanf("%d", &frames);
    printf("Enter number of page references: ");  scanf("%d", &n);
    printf("Enter page reference string:\n");
    for (i = 0; i < n; i++) scanf("%d", &pages[i]);
    for (i = 0; i < frames; i++) { frame[i] = -1; last_used[i] = 0; }

    printf("\nPage\t|\tFrames\t\t\t|Fault\n");
    printf("----\t|\t-------\t\t\t|------\n");
    for (i = 0; i < n; i++) {
        int found = 0, hit_idx = -1;
        for (j = 0; j < frames; j++)
            if (frame[j] == pages[i]) { found = 1; hit_idx = j; break; }

        printf("%d\t|\t", pages[i]);
        if (!found) {
            int replace = 0, has_empty = 0;
            for (j = 0; j < frames; j++)
                if (frame[j] == -1) { replace = j; has_empty = 1; break; }
            if (!has_empty)
                for (j = 1; j < frames; j++)
                    if (last_used[j] < last_used[replace]) replace = j;
            frame[replace]     = pages[i];
            last_used[replace] = i + 1;
            faults++;
            for (j = 0; j < frames; j++) print_frame(frame[j]);
            printf("\t\t|FAULT\n");
        } else {
            last_used[hit_idx] = i + 1;
            for (j = 0; j < frames; j++) print_frame(frame[j]);
            printf("\t\t|No Fault\n");
        }
    }
    printf("\nTotal Page Faults : %d\n", faults);
    printf("Hit Ratio         : %.2f\n", (float)(n - faults) / n);
}

/* ============================================================
   EXPERIMENT 18: Disk Scheduling (FCFS, SCAN, C-SCAN)
   ============================================================ */
void disk_scheduling() {
    int n, head, i, j, total, cur;
    int req[20], sorted[20];

    printf("\n========== EXP 18: Disk Scheduling ==========\n");
    printf("Enter number of disk requests: "); scanf("%d", &n);
    printf("Enter disk requests:\n");
    for (i = 0; i < n; i++) scanf("%d", &req[i]);
    printf("Enter initial head position: ");   scanf("%d", &head);

    /* FCFS */
    printf("\n--- FCFS ---\n");
    total = 0; cur = head;
    printf("Sequence: %d", cur);
    for (i = 0; i < n; i++) {
        total += iabs(req[i] - cur);
        cur    = req[i];
        printf(" -> %d", cur);
    }
    printf("\nTotal Head Movement: %d\n", total);

    /* Sort for SCAN / C-SCAN */
    for (i = 0; i < n; i++) sorted[i] = req[i];
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (sorted[j] > sorted[j + 1]) swap_int(&sorted[j], &sorted[j + 1]);

    /* SCAN */
    printf("\n--- SCAN (higher numbers first) ---\n");
    total = 0; cur = head;
    printf("Sequence: %d", cur);
    for (i = 0; i < n; i++)
        if (sorted[i] >= head) { total += iabs(sorted[i]-cur); cur=sorted[i]; printf(" -> %d",cur); }
    for (i = n-1; i >= 0; i--)
        if (sorted[i] < head)  { total += iabs(sorted[i]-cur); cur=sorted[i]; printf(" -> %d",cur); }
    printf("\nTotal Head Movement: %d\n", total);

    /* C-SCAN */
    printf("\n--- C-SCAN ---\n");
    int disk_size = 200;
    total = 0; cur = head;
    printf("Sequence: %d", cur);
    for (i = 0; i < n; i++)
        if (sorted[i] >= head) { total += iabs(sorted[i]-cur); cur=sorted[i]; printf(" -> %d",cur); }
    total += iabs(disk_size - cur) + disk_size;
    cur = 0;
    printf(" -> %d -> 0", disk_size);
    for (i = 0; i < n; i++)
        if (sorted[i] < head)  { total += iabs(sorted[i]-cur); cur=sorted[i]; printf(" -> %d",cur); }
    printf("\nTotal Head Movement: %d\n", total);
}

/* ============================================================
   MAIN MENU
   ============================================================ */
int main() {
    int choice;
    do {
        printf("\n============================================\n");
        printf(" SIES OS Lab - CSEL402 - Dev C++ Programs\n");
        printf("============================================\n");
        printf("  5.  FCFS Scheduling\n");
        printf("  6.  Round Robin Scheduling\n");
        printf("  9.  Producer-Consumer (Semaphore)\n");
        printf(" 10.  Banker's Algorithm\n");
        printf(" 11.  MFT Memory Management\n");
        printf(" 12.  Best Fit\n");
        printf(" 13.  First Fit\n");
        printf(" 14.  Worst Fit\n");
        printf(" 15.  FIFO Page Replacement\n");
        printf(" 16.  LIFO Page Replacement\n");
        printf(" 17.  LRU Page Replacement\n");
        printf(" 18.  Disk Scheduling (FCFS/SCAN/C-SCAN)\n");
        printf("  0.  Exit\n");
        printf("============================================\n");
        printf("Enter experiment number: ");
        scanf("%d", &choice);

        switch (choice) {
            case 5:  fcfs_scheduling();        break;
            case 6:  round_robin_scheduling(); break;
            case 9:  producer_consumer();      break;
            case 10: bankers_algorithm();      break;
            case 11: mft_memory();             break;
            case 12: best_fit();               break;
            case 13: first_fit();              break;
            case 14: worst_fit();              break;
            case 15: fifo_page_replacement();  break;
            case 16: lifo_page_replacement();  break;
            case 17: lru_page_replacement();   break;
            case 18: disk_scheduling();        break;
            case 0:  printf("Exiting...\n");   break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}
