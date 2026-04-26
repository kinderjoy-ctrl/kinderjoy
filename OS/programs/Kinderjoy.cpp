/*
============================================================
  SIES Graduate School of Technology
  OS Lab (CSEL402) - Programs executable on Dev C++ / Windows
  Course Incharge: Ms. Pooja Phad
============================================================

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

#include <iostream>
#include <climits>
#include <cmath>
using namespace std;

// ============================================================
// EXPERIMENT 5: FCFS (Non-Preemptive) CPU Scheduling
// ============================================================
void fcfs_scheduling() {
    cout << "\n========== EXP 5: FCFS Scheduling ==========\n";
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    int pid[20], bt[20], at[20];
    int wt[20], tat[20], ct[20];

    cout << "Enter Arrival Time and Burst Time for each process:\n";
    for (int i = 0; i < n; i++) {
        pid[i] = i + 1;
        cout << "P" << pid[i] << " Arrival Time: ";
        cin >> at[i];
        cout << "P" << pid[i] << " Burst Time: ";
        cin >> bt[i];
    }

    // Sort by arrival time (simple bubble sort)
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (at[j] > at[j + 1]) {
                swap(at[j], at[j + 1]);
                swap(bt[j], bt[j + 1]);
                swap(pid[j], pid[j + 1]);
            }

    ct[0] = at[0] + bt[0];
    for (int i = 1; i < n; i++)
        ct[i] = (ct[i - 1] > at[i] ? ct[i - 1] : at[i]) + bt[i];

    float total_wt = 0, total_tat = 0;
    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for (int i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        total_wt += wt[i];
        total_tat += tat[i];
        cout << "P" << pid[i] << "\t" << at[i] << "\t" << bt[i]
             << "\t" << ct[i] << "\t" << tat[i] << "\t" << wt[i] << "\n";
    }
    cout << "Average Waiting Time    : " << total_wt / n << "\n";
    cout << "Average Turnaround Time : " << total_tat / n << "\n";
}

// ============================================================
// EXPERIMENT 6: Round Robin (Preemptive) Scheduling
// ============================================================
void round_robin_scheduling() {
    cout << "\n========== EXP 6: Round Robin Scheduling ==========\n";
    int n, tq;
    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter Time Quantum: ";
    cin >> tq;

    int bt[20], at[20], rem[20], ct[20];
    int wt[20], tat[20];

    cout << "Enter Arrival Time and Burst Time:\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << i + 1 << " Arrival Time: "; cin >> at[i];
        cout << "P" << i + 1 << " Burst Time: ";   cin >> bt[i];
        rem[i] = bt[i];
        ct[i] = 0;
    }

    int time = 0, done = 0;
    while (done < n) {
        bool any = false;
        for (int i = 0; i < n; i++) {
            if (rem[i] > 0 && at[i] <= time) {
                any = true;
                if (rem[i] <= tq) {
                    time += rem[i];
                    rem[i] = 0;
                    ct[i] = time;
                    done++;
                } else {
                    rem[i] -= tq;
                    time += tq;
                }
            }
        }
        if (!any) time++;
    }

    float total_wt = 0, total_tat = 0;
    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for (int i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        total_wt += wt[i];
        total_tat += tat[i];
        cout << "P" << i + 1 << "\t" << at[i] << "\t" << bt[i]
             << "\t" << ct[i] << "\t" << tat[i] << "\t" << wt[i] << "\n";
    }
    cout << "Average Waiting Time    : " << total_wt / n << "\n";
    cout << "Average Turnaround Time : " << total_tat / n << "\n";
}

// ============================================================
// EXPERIMENT 9: Producer-Consumer using Semaphore (Simulated)
// ============================================================
// Note: True semaphores require OS threads. This simulates the
// logic of semaphore-controlled bounded buffer in C++.
void producer_consumer() {
    cout << "\n========== EXP 9: Producer-Consumer (Semaphore Simulation) ==========\n";
    int buffer_size;
    cout << "Enter buffer size: ";
    cin >> buffer_size;

    int buffer[20];
    int in = 0, out = 0;
    int empty_sem = buffer_size; // semaphore: empty slots
    int full_sem = 0;            // semaphore: filled slots
    int mutex = 1;               // binary semaphore

    int choice, item;
    do {
        cout << "\n1. Produce\n2. Consume\n3. Display Buffer\n0. Exit\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                if (empty_sem == 0) {
                    cout << "Buffer FULL! Producer must wait.\n";
                } else {
                    cout << "Enter item to produce: ";
                    cin >> item;
                    // wait(empty), wait(mutex)
                    empty_sem--;
                    mutex = 0;
                    buffer[in] = item;
                    in = (in + 1) % buffer_size;
                    // signal(mutex), signal(full)
                    mutex = 1;
                    full_sem++;
                    cout << "Produced item: " << item << " | Empty=" << empty_sem << " Full=" << full_sem << "\n";
                }
                break;
            case 2:
                if (full_sem == 0) {
                    cout << "Buffer EMPTY! Consumer must wait.\n";
                } else {
                    // wait(full), wait(mutex)
                    full_sem--;
                    mutex = 0;
                    item = buffer[out];
                    out = (out + 1) % buffer_size;
                    // signal(mutex), signal(empty)
                    mutex = 1;
                    empty_sem++;
                    cout << "Consumed item: " << item << " | Empty=" << empty_sem << " Full=" << full_sem << "\n";
                }
                break;
            case 3:
                cout << "Buffer contents: ";
                for (int i = out; i != in; i = (i + 1) % buffer_size)
                    cout << buffer[i] << " ";
                cout << "\n";
                break;
        }
    } while (choice != 0);
}

// ============================================================
// EXPERIMENT 10: Banker's Algorithm (Deadlock Avoidance)
// ============================================================
void bankers_algorithm() {
    cout << "\n========== EXP 10: Banker's Algorithm ==========\n";
    int n, m;
    cout << "Enter number of processes: "; cin >> n;
    cout << "Enter number of resource types: "; cin >> m;

    int alloc[10][10], maxm[10][10], avail[10];
    int need[10][10];

    cout << "Enter Allocation Matrix:\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) cin >> alloc[i][j];

    cout << "Enter Max Matrix:\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) cin >> maxm[i][j];

    cout << "Enter Available Resources:\n";
    for (int j = 0; j < m; j++) cin >> avail[j];

    // Calculate need matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = maxm[i][j] - alloc[i][j];

    // Safety Algorithm
    bool finish[10] = {false};
    int work[10], safeSeq[10], count = 0;
    for (int j = 0; j < m; j++) work[j] = avail[j];

    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool possible = true;
                for (int j = 0; j < m; j++)
                    if (need[i][j] > work[j]) { possible = false; break; }
                if (possible) {
                    for (int j = 0; j < m; j++) work[j] += alloc[i][j];
                    safeSeq[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) break;
    }

    if (count == n) {
        cout << "\nSystem is in SAFE STATE.\nSafe Sequence: ";
        for (int i = 0; i < n; i++) cout << "P" << safeSeq[i] << (i < n - 1 ? " -> " : "\n");
    } else {
        cout << "\nSystem is in UNSAFE STATE! Deadlock may occur.\n";
    }

    // Resource Request Check
    char ch;
    cout << "\nCheck resource request? (y/n): "; cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        int pid;
        cout << "Enter process number (0-indexed): "; cin >> pid;
        int req[10];
        cout << "Enter request vector: ";
        for (int j = 0; j < m; j++) cin >> req[j];

        bool valid = true;
        for (int j = 0; j < m; j++)
            if (req[j] > need[pid][j] || req[j] > avail[j]) { valid = false; break; }

        if (valid) {
            for (int j = 0; j < m; j++) {
                avail[j] -= req[j];
                alloc[pid][j] += req[j];
                need[pid][j] -= req[j];
            }
            cout << "Request can be GRANTED. System remains safe.\n";
        } else {
            cout << "Request DENIED. Would lead to unsafe state.\n";
        }
    }
}

// ============================================================
// EXPERIMENT 11: MFT (Multiprogramming with Fixed Tasks)
// ============================================================
void mft_memory() {
    cout << "\n========== EXP 11: MFT Memory Management ==========\n";
    int total, n, os_size;
    cout << "Enter total memory size (KB): "; cin >> total;
    cout << "Enter OS size (KB): "; cin >> os_size;
    int user_mem = total - os_size;
    cout << "Enter number of fixed partitions: "; cin >> n;

    int part_size = user_mem / n;
    cout << "\nEach partition size: " << part_size << " KB\n";

    int jobs[20], job_size[20];
    cout << "Enter number of jobs: ";
    int jobs_n; cin >> jobs_n;
    for (int i = 0; i < jobs_n; i++) {
        cout << "Job " << i + 1 << " size (KB): "; cin >> job_size[i];
        jobs[i] = i + 1;
    }

    bool occupied[20] = {false};
    cout << "\nPartition\tSize\tJob\t\tInternal Fragmentation\n";
    for (int i = 0; i < n; i++) {
        bool alloc = false;
        for (int j = 0; j < jobs_n; j++) {
            if (!occupied[j] && job_size[j] <= part_size) {
                cout << i + 1 << "\t\t" << part_size << "\tJob " << jobs[j]
                     << "(" << job_size[j] << "KB)\t" << part_size - job_size[j] << " KB\n";
                occupied[j] = true;
                alloc = true;
                break;
            }
        }
        if (!alloc)
            cout << i + 1 << "\t\t" << part_size << "\tEmpty\t\t" << part_size << " KB\n";
    }
}

// ============================================================
// EXPERIMENT 12: Best Fit Dynamic Partitioning
// ============================================================
void best_fit() {
    cout << "\n========== EXP 12: Best Fit Memory Allocation ==========\n";
    int n, m;
    cout << "Enter number of memory blocks: "; cin >> n;
    cout << "Enter number of processes: "; cin >> m;

    int block[20], process[20], block_copy[20];
    cout << "Enter block sizes:\n";
    for (int i = 0; i < n; i++) { cin >> block[i]; block_copy[i] = block[i]; }
    cout << "Enter process sizes:\n";
    for (int i = 0; i < m; i++) cin >> process[i];

    int alloc[20];
    for (int i = 0; i < m; i++) alloc[i] = -1;

    for (int i = 0; i < m; i++) {
        int best = -1;
        for (int j = 0; j < n; j++) {
            if (block[j] >= process[i]) {
                if (best == -1 || block[j] < block[best]) best = j;
            }
        }
        if (best != -1) { alloc[i] = best; block[best] -= process[i]; }
    }

    cout << "\nProcess\tSize\tBlock Alloc\tBlock Size\tFragmentation\n";
    for (int i = 0; i < m; i++) {
        cout << i + 1 << "\t" << process[i] << "\t";
        if (alloc[i] != -1)
            cout << alloc[i] + 1 << "\t\t" << block_copy[alloc[i]] << "\t\t" << block_copy[alloc[i]] - process[i] << "\n";
        else
            cout << "Not Allocated\n";
    }
}

// ============================================================
// EXPERIMENT 13: First Fit Dynamic Partitioning
// ============================================================
void first_fit() {
    cout << "\n========== EXP 13: First Fit Memory Allocation ==========\n";
    int n, m;
    cout << "Enter number of memory blocks: "; cin >> n;
    cout << "Enter number of processes: "; cin >> m;

    int block[20], process[20], block_copy[20];
    cout << "Enter block sizes:\n";
    for (int i = 0; i < n; i++) { cin >> block[i]; block_copy[i] = block[i]; }
    cout << "Enter process sizes:\n";
    for (int i = 0; i < m; i++) cin >> process[i];

    int alloc[20];
    for (int i = 0; i < m; i++) alloc[i] = -1;

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (block[j] >= process[i]) {
                alloc[i] = j;
                block[j] -= process[i];
                break;
            }

    cout << "\nProcess\tSize\tBlock Alloc\tBlock Size\tFragmentation\n";
    for (int i = 0; i < m; i++) {
        cout << i + 1 << "\t" << process[i] << "\t";
        if (alloc[i] != -1)
            cout << alloc[i] + 1 << "\t\t" << block_copy[alloc[i]] << "\t\t" << block_copy[alloc[i]] - process[i] << "\n";
        else
            cout << "Not Allocated\n";
    }
}

// ============================================================
// EXPERIMENT 14: Worst Fit Dynamic Partitioning
// ============================================================
void worst_fit() {
    cout << "\n========== EXP 14: Worst Fit Memory Allocation ==========\n";
    int n, m;
    cout << "Enter number of memory blocks: "; cin >> n;
    cout << "Enter number of processes: "; cin >> m;

    int block[20], process[20], block_copy[20];
    cout << "Enter block sizes:\n";
    for (int i = 0; i < n; i++) { cin >> block[i]; block_copy[i] = block[i]; }
    cout << "Enter process sizes:\n";
    for (int i = 0; i < m; i++) cin >> process[i];

    int alloc[20];
    for (int i = 0; i < m; i++) alloc[i] = -1;

    for (int i = 0; i < m; i++) {
        int worst = -1;
        for (int j = 0; j < n; j++) {
            if (block[j] >= process[i]) {
                if (worst == -1 || block[j] > block[worst]) worst = j;
            }
        }
        if (worst != -1) { alloc[i] = worst; block[worst] -= process[i]; }
    }

    cout << "\nProcess\tSize\tBlock Alloc\tBlock Size\tFragmentation\n";
    for (int i = 0; i < m; i++) {
        cout << i + 1 << "\t" << process[i] << "\t";
        if (alloc[i] != -1)
            cout << alloc[i] + 1 << "\t\t" << block_copy[alloc[i]] << "\t\t" << block_copy[alloc[i]] - process[i] << "\n";
        else
            cout << "Not Allocated\n";
    }
}

// ============================================================
// EXPERIMENT 15: FIFO Page Replacement
// ============================================================
void fifo_page_replacement() {
    cout << "\n========== EXP 15: FIFO Page Replacement ==========\n";
    int frames, n;
    cout << "Enter number of frames: "; cin >> frames;
    cout << "Enter number of page references: "; cin >> n;

    int pages[50];
    cout << "Enter page reference string:\n";
    for (int i = 0; i < n; i++) cin >> pages[i];

    int frame[10], pointer = 0, faults = 0;
    for (int i = 0; i < frames; i++) frame[i] = -1;

    cout << "\nPage\t|\tFrames\t\t\t|Fault\n";
    cout << "----\t|\t-------\t\t\t|------\n";
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < frames; j++)
            if (frame[j] == pages[i]) { found = true; break; }

        cout << pages[i] << "\t|\t";
        if (!found) {
            frame[pointer] = pages[i];
            pointer = (pointer + 1) % frames;
            faults++;
            for (int j = 0; j < frames; j++) cout << (frame[j] == -1 ? "-" : to_string(frame[j])) << " ";
            cout << "\t\t|FAULT\n";
        } else {
            for (int j = 0; j < frames; j++) cout << (frame[j] == -1 ? "-" : to_string(frame[j])) << " ";
            cout << "\t\t|No Fault\n";
        }
    }
    cout << "\nTotal Page Faults: " << faults << "\n";
    cout << "Hit Ratio: " << (float)(n - faults) / n << "\n";
}

// ============================================================
// EXPERIMENT 16: LIFO Page Replacement
// ============================================================
void lifo_page_replacement() {
    cout << "\n========== EXP 16: LIFO Page Replacement ==========\n";
    int frames, n;
    cout << "Enter number of frames: "; cin >> frames;
    cout << "Enter number of page references: "; cin >> n;

    int pages[50];
    cout << "Enter page reference string:\n";
    for (int i = 0; i < n; i++) cin >> pages[i];

    int frame[10], top = 0, faults = 0;
    bool frame_full[10] = {false};
    for (int i = 0; i < frames; i++) frame[i] = -1;

    cout << "\nPage\t|\tFrames\t\t\t|Fault\n";
    cout << "----\t|\t-------\t\t\t|------\n";
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < frames; j++)
            if (frame[j] == pages[i]) { found = true; break; }

        cout << pages[i] << "\t|\t";
        if (!found) {
            bool hasEmpty = false;
            for (int j = 0; j < frames; j++)
                if (frame[j] == -1) { frame[j] = pages[i]; top = j; hasEmpty = true; break; }
            if (!hasEmpty) {
                frame[top] = pages[i]; // Replace last added (LIFO)
            }
            faults++;
            for (int j = 0; j < frames; j++) cout << (frame[j] == -1 ? "-" : to_string(frame[j])) << " ";
            cout << "\t\t|FAULT\n";
        } else {
            for (int j = 0; j < frames; j++) cout << (frame[j] == -1 ? "-" : to_string(frame[j])) << " ";
            cout << "\t\t|No Fault\n";
        }
    }
    cout << "\nTotal Page Faults: " << faults << "\n";
    cout << "Hit Ratio: " << (float)(n - faults) / n << "\n";
}

// ============================================================
// EXPERIMENT 17: LRU Page Replacement
// ============================================================
void lru_page_replacement() {
    cout << "\n========== EXP 17: LRU Page Replacement ==========\n";
    int frames, n;
    cout << "Enter number of frames: "; cin >> frames;
    cout << "Enter number of page references: "; cin >> n;

    int pages[50];
    cout << "Enter page reference string:\n";
    for (int i = 0; i < n; i++) cin >> pages[i];

    int frame[10], last_used[10], faults = 0;
    for (int i = 0; i < frames; i++) { frame[i] = -1; last_used[i] = 0; }

    cout << "\nPage\t|\tFrames\t\t\t|Fault\n";
    cout << "----\t|\t-------\t\t\t|------\n";
    for (int i = 0; i < n; i++) {
        bool found = false;
        int hit_idx = -1;
        for (int j = 0; j < frames; j++)
            if (frame[j] == pages[i]) { found = true; hit_idx = j; break; }

        cout << pages[i] << "\t|\t";
        if (!found) {
            int replace = 0;
            // Find empty slot first
            bool empty = false;
            for (int j = 0; j < frames; j++)
                if (frame[j] == -1) { replace = j; empty = true; break; }
            // Else find LRU
            if (!empty)
                for (int j = 1; j < frames; j++)
                    if (last_used[j] < last_used[replace]) replace = j;

            frame[replace] = pages[i];
            last_used[replace] = i + 1;
            faults++;
            for (int j = 0; j < frames; j++) cout << (frame[j] == -1 ? "-" : to_string(frame[j])) << " ";
            cout << "\t\t|FAULT\n";
        } else {
            last_used[hit_idx] = i + 1;
            for (int j = 0; j < frames; j++) cout << (frame[j] == -1 ? "-" : to_string(frame[j])) << " ";
            cout << "\t\t|No Fault\n";
        }
    }
    cout << "\nTotal Page Faults: " << faults << "\n";
    cout << "Hit Ratio: " << (float)(n - faults) / n << "\n";
}

// ============================================================
// EXPERIMENT 18: Disk Scheduling (FCFS, SCAN, C-SCAN)
// ============================================================
void disk_scheduling() {
    cout << "\n========== EXP 18: Disk Scheduling ==========\n";
    int n, head;
    cout << "Enter number of disk requests: "; cin >> n;
    int req[20];
    cout << "Enter disk requests:\n";
    for (int i = 0; i < n; i++) cin >> req[i];
    cout << "Enter initial head position: "; cin >> head;

    // --- FCFS ---
    cout << "\n--- FCFS ---\n";
    int total = 0, cur = head;
    cout << "Sequence: " << cur;
    for (int i = 0; i < n; i++) {
        total += abs(req[i] - cur);
        cur = req[i];
        cout << " -> " << cur;
    }
    cout << "\nTotal Head Movement: " << total << "\n";

    // --- SCAN ---
    cout << "\n--- SCAN (direction: higher first) ---\n";
    int sorted[20];
    for (int i = 0; i < n; i++) sorted[i] = req[i];
    // Sort ascending
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (sorted[j] > sorted[j + 1]) swap(sorted[j], sorted[j + 1]);

    total = 0; cur = head;
    cout << "Sequence: " << cur;
    // Go right first
    for (int i = 0; i < n; i++)
        if (sorted[i] >= head) {
            total += abs(sorted[i] - cur);
            cur = sorted[i];
            cout << " -> " << cur;
        }
    // Then go left
    for (int i = n - 1; i >= 0; i--)
        if (sorted[i] < head) {
            total += abs(sorted[i] - cur);
            cur = sorted[i];
            cout << " -> " << cur;
        }
    cout << "\nTotal Head Movement: " << total << "\n";

    // --- C-SCAN ---
    cout << "\n--- C-SCAN ---\n";
    total = 0; cur = head;
    int disk_size = 200; // default max cylinder
    cout << "Sequence: " << cur;
    // Go right to end
    for (int i = 0; i < n; i++)
        if (sorted[i] >= head) {
            total += abs(sorted[i] - cur);
            cur = sorted[i];
            cout << " -> " << cur;
        }
    // Jump to start
    total += abs(disk_size - cur) + disk_size;
    cur = 0;
    cout << " -> " << disk_size << " -> 0";
    // Continue from start
    for (int i = 0; i < n; i++)
        if (sorted[i] < head) {
            total += abs(sorted[i] - cur);
            cur = sorted[i];
            cout << " -> " << cur;
        }
    cout << "\nTotal Head Movement: " << total << "\n";
}

// ============================================================
// MAIN MENU
// ============================================================
int main() {
    int choice;
    do {
        cout << "\n============================================\n";
        cout << " SIES OS Lab - CSEL402 - Dev C++ Programs\n";
        cout << "============================================\n";
        cout << " 5.  FCFS Scheduling\n";
        cout << " 6.  Round Robin Scheduling\n";
        cout << " 9.  Producer-Consumer (Semaphore)\n";
        cout << " 10. Banker's Algorithm\n";
        cout << " 11. MFT Memory Management\n";
        cout << " 12. Best Fit\n";
        cout << " 13. First Fit\n";
        cout << " 14. Worst Fit\n";
        cout << " 15. FIFO Page Replacement\n";
        cout << " 16. LIFO Page Replacement\n";
        cout << " 17. LRU Page Replacement\n";
        cout << " 18. Disk Scheduling (FCFS/SCAN/C-SCAN)\n";
        cout << "  0. Exit\n";
        cout << "============================================\n";
        cout << "Enter experiment number: ";
        cin >> choice;

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
            case 0:  cout << "Exiting...\n";   break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
