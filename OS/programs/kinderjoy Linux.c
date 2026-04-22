/*
============================================================
  SIES Graduate School of Technology
  OS Lab (CSEL402) - Linux-Only Programs
  Course Incharge: Ms. Pooja Phad
  Class/Sem: SE/IV  |  Lab: Lab 6

  HOW TO COMPILE & RUN (Ubuntu/Linux Terminal):
    gcc linux_os_lab.c -o oslab
    ./oslab

  PROGRAMS INCLUDED:
    Exp 1 - Basic Linux Commands & System Calls (read/open/close/getpid/getuid/getgid)
    Exp 2 - Basic Linux Commands & System Calls (write/getegid/geteuid/sort)
    Exp 3 - Shell Scripts (OS version, top10 processes, memory, user, env info)
    Exp 4 - fork(), getpid(), getppid(), wait(), waitpid()
    Exp 7 - (CPU-OS Simulator note - see docx for instructions)
    Exp 8 - Thread Creation & Synchronization (pthreads)
============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

/* ============================================================
   EXPERIMENT 1
   Basic Linux Commands & System Calls
   System calls: open, read, close, getpid, getuid, getgid
   ============================================================ */
void exp1_system_calls_read() {
    printf("\n========== EXP 1: System Calls - open/read/close/getpid/getuid/getgid ==========\n");

    /* --- getpid, getuid, getgid --- */
    printf("\n[Process Info via System Calls]\n");
    printf("Process ID  (getpid) : %d\n", getpid());
    printf("User ID     (getuid) : %d\n", getuid());
    printf("Group ID    (getgid) : %d\n", getgid());

    /* --- Create a test file, then open/read/close --- */
    const char *filename = "/tmp/oslab_exp1.txt";
    char write_buf[] = "Hello from SIES OS Lab Experiment 1!\nSystem call: open, read, close\n";
    char read_buf[256];

    /* Write using standard C (for setup) */
    FILE *fp = fopen(filename, "w");
    if (fp) { fputs(write_buf, fp); fclose(fp); }

    /* Open using open() system call */
    int fd = open(filename, O_RDONLY);
    if (fd < 0) { perror("open failed"); return; }
    printf("\n[File Operations via System Calls]\n");
    printf("File opened successfully. fd = %d\n", fd);

    /* Read using read() system call */
    ssize_t bytes = read(fd, read_buf, sizeof(read_buf) - 1);
    if (bytes < 0) { perror("read failed"); close(fd); return; }
    read_buf[bytes] = '\0';
    printf("Bytes read: %zd\n", bytes);
    printf("Content   : %s", read_buf);

    /* Close using close() system call */
    close(fd);
    printf("File closed successfully.\n");
}

/* ============================================================
   EXPERIMENT 2
   Basic Linux Commands & System Calls
   System calls: open, write, close, getegid, geteuid, sort
   ============================================================ */
void exp2_system_calls_write() {
    printf("\n========== EXP 2: System Calls - open/write/close/geteuid/getegid ==========\n");

    printf("\n[Effective IDs via System Calls]\n");
    printf("Effective User ID  (geteuid) : %d\n", geteuid());
    printf("Effective Group ID (getegid) : %d\n", getegid());
    printf("Process ID         (getpid)  : %d\n", getpid());

    /* --- open with O_WRONLY | O_CREAT, write, close --- */
    const char *filename = "/tmp/oslab_exp2.txt";
    char buf[] = "SIES OS Lab Experiment 2\nSystem call: open (write), write, close\nLine 3: sorting demo below\n";

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open failed"); return; }
    printf("\n[File Write via System Calls]\n");
    printf("File created/opened. fd = %d\n", fd);

    ssize_t written = write(fd, buf, strlen(buf));
    printf("Bytes written: %zd\n", written);
    close(fd);
    printf("File closed.\n");

    /* --- Demonstrate sort via system() --- */
    printf("\n[sort command demo]\n");
    printf("Unsorted: banana apple cherry date elderberry\n");
    printf("Sorted output:\n");
    system("echo -e 'banana\napple\ncherry\ndate\nelderberry' | sort");
}

/* ============================================================
   EXPERIMENT 3
   Shell Script Demonstrations (executed via system())
   a. OS version, release, kernel
   b. Top 10 processes descending
   c. Highest memory usage
   d. Current logged-in user and logname
   e. Shell, home dir, OS type, PATH, CWD
   ============================================================ */
void exp3_shell_scripts() {
    printf("\n========== EXP 3: Shell Script Demonstrations ==========\n");

    printf("\n--- (a) OS Version, Release Number, Kernel Version ---\n");
    system("echo 'OS Version:'; uname -o 2>/dev/null || uname -s");
    system("echo 'Release:'; uname -r");
    system("echo 'Kernel Version:'; uname -v");
    system("echo 'Full uname -a:'; uname -a");
    system("cat /etc/os-release 2>/dev/null | grep -E 'NAME|VERSION' | head -4");

    printf("\n--- (b) Top 10 Processes in Descending Order (by CPU) ---\n");
    system("ps aux --sort=-%cpu 2>/dev/null | head -11 || ps aux | head -11");

    printf("\n--- (c) Processes with Highest Memory Usage ---\n");
    system("ps aux --sort=-%mem 2>/dev/null | head -6 || ps aux | sort -k4 -rn | head -6");

    printf("\n--- (d) Current Logged-In User and Logname ---\n");
    system("echo 'whoami  :'; whoami");
    system("echo 'logname :'; logname 2>/dev/null || echo '(logname not available in this session)'");
    system("echo 'who     :'; who 2>/dev/null | head -3");
    system("echo 'id      :'; id");

    printf("\n--- (e) Shell, Home Dir, OS Type, PATH, CWD ---\n");
    system("echo 'Current Shell    : '$SHELL");
    system("echo 'Home Directory   : '$HOME");
    system("echo 'OS Type (uname)  : '$(uname -s)");
    system("echo 'Current PATH     : '$PATH");
    system("echo 'Working Dir (pwd): '$(pwd)");
}

/* ============================================================
   EXPERIMENT 4
   fork(), getpid(), getppid(), wait(), waitpid()
   ============================================================ */
void exp4_fork_and_wait() {
    printf("\n========== EXP 4: fork(), getpid(), getppid(), wait(), waitpid() ==========\n");

    printf("\n--- Part (a): fork() with getpid() and getppid() ---\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork() failed");
        return;
    }
    else if (pid == 0) {
        /* CHILD PROCESS */
        printf("\n[CHILD PROCESS]\n");
        printf("  Child PID  (getpid())  = %d\n", getpid());
        printf("  Parent PID (getppid()) = %d\n", getppid());
        printf("  Child is about to exit.\n");
        exit(0);
    }
    else {
        /* PARENT PROCESS */
        printf("\n[PARENT PROCESS]\n");
        printf("  Parent PID (getpid())  = %d\n", getpid());
        printf("  Child PID returned by fork() = %d\n", pid);

        printf("\n--- Part (b): wait() before termination ---\n");
        int status;
        pid_t waited = wait(&status);
        printf("  wait() returned. Child PID = %d\n", waited);
        if (WIFEXITED(status))
            printf("  Child exited normally with status: %d\n", WEXITSTATUS(status));

        /* waitpid() demo with a second child */
        printf("\n--- waitpid() demo ---\n");
        pid_t pid2 = fork();
        if (pid2 == 0) {
            printf("[CHILD2] PID=%d, sleeping 1 sec...\n", getpid());
            sleep(1);
            printf("[CHILD2] Done.\n");
            exit(42);
        } else {
            printf("[PARENT] Waiting for Child2 (PID=%d) using waitpid()...\n", pid2);
            pid_t wp = waitpid(pid2, &status, 0);
            printf("[PARENT] waitpid() returned: %d\n", wp);
            if (WIFEXITED(status))
                printf("[PARENT] Child2 exit code: %d\n", WEXITSTATUS(status));
        }
    }
}

/* ============================================================
   EXPERIMENT 8
   Thread Creation & Synchronization using pthreads
   Compile with: gcc linux_os_lab.c -o oslab -lpthread
   ============================================================ */

#define NUM_THREADS 4
#define BUFFER_SIZE 5

/* Shared data for synchronization demo */
int shared_counter = 0;
pthread_mutex_t mutex_lock;
pthread_cond_t  cond_var;

/* Simple thread function */
void *thread_func(void *arg) {
    int tid = *(int *)arg;
    printf("  [Thread %d] started. TID = %lu\n", tid, (unsigned long)pthread_self());

    pthread_mutex_lock(&mutex_lock);
    shared_counter++;
    printf("  [Thread %d] incremented shared_counter to %d\n", tid, shared_counter);
    pthread_mutex_unlock(&mutex_lock);

    /* Signal condition if counter reaches threshold */
    if (shared_counter == NUM_THREADS) {
        pthread_cond_signal(&cond_var);
    }

    pthread_exit(NULL);
}

/* Producer-Consumer with mutex + condition variable */
int prod_cons_buffer[BUFFER_SIZE];
int prod_in = 0, prod_out = 0, prod_count = 0;
pthread_mutex_t pc_mutex;
pthread_cond_t  not_full, not_empty;

void *producer(void *arg) {
    for (int i = 1; i <= 5; i++) {
        pthread_mutex_lock(&pc_mutex);
        while (prod_count == BUFFER_SIZE)
            pthread_cond_wait(&not_full, &pc_mutex);
        prod_cons_buffer[prod_in] = i * 10;
        prod_in = (prod_in + 1) % BUFFER_SIZE;
        prod_count++;
        printf("  [Producer] Produced: %d  | Buffer count: %d\n", i * 10, prod_count);
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&pc_mutex);
        usleep(100000);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&pc_mutex);
        while (prod_count == 0)
            pthread_cond_wait(&not_empty, &pc_mutex);
        int item = prod_cons_buffer[prod_out];
        prod_out = (prod_out + 1) % BUFFER_SIZE;
        prod_count--;
        printf("  [Consumer] Consumed: %d  | Buffer count: %d\n", item, prod_count);
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&pc_mutex);
        usleep(150000);
    }
    return NULL;
}

void exp8_threads() {
    printf("\n========== EXP 8: Thread Creation & Synchronization (pthreads) ==========\n");

    /* --- Part A: Basic thread creation with mutex --- */
    printf("\n--- Part A: Thread Creation with Mutex (shared counter) ---\n");
    pthread_t threads[NUM_THREADS];
    int tids[NUM_THREADS];
    shared_counter = 0;

    pthread_mutex_init(&mutex_lock, NULL);
    pthread_cond_init(&cond_var, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        tids[i] = i + 1;
        if (pthread_create(&threads[i], NULL, thread_func, &tids[i]) != 0)
            perror("pthread_create failed");
    }
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("  All threads done. Final shared_counter = %d\n", shared_counter);
    pthread_mutex_destroy(&mutex_lock);
    pthread_cond_destroy(&cond_var);

    /* --- Part B: Producer-Consumer with condition variables --- */
    printf("\n--- Part B: Producer-Consumer with Condition Variables ---\n");
    pthread_t prod_tid, cons_tid;
    prod_in = prod_out = prod_count = 0;

    pthread_mutex_init(&pc_mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    pthread_create(&prod_tid, NULL, producer, NULL);
    pthread_create(&cons_tid, NULL, consumer, NULL);
    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    pthread_mutex_destroy(&pc_mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);
    printf("  Producer-Consumer demo complete.\n");
}

/* ============================================================
   MAIN MENU
   ============================================================ */
int main() {
    int choice;
    do {
        printf("\n============================================\n");
        printf(" SIES OS Lab - CSEL402 - Linux Programs\n");
        printf("============================================\n");
        printf(" 1. System Calls: open/read/close/getpid/getuid/getgid\n");
        printf(" 2. System Calls: open/write/close/geteuid/getegid/sort\n");
        printf(" 3. Shell Script Demonstrations\n");
        printf(" 4. fork(), getpid(), getppid(), wait(), waitpid()\n");
        printf(" 7. (CPU-OS Simulator - see DOCX instructions)\n");
        printf(" 8. Thread Creation & Synchronization (pthreads)\n");
        printf(" 0. Exit\n");
        printf("============================================\n");
        printf("Enter experiment number: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: exp1_system_calls_read();  break;
            case 2: exp2_system_calls_write(); break;
            case 3: exp3_shell_scripts();      break;
            case 4: exp4_fork_and_wait();      break;
            case 7:
                printf("\nExp 7 requires the CPU-OS Simulator GUI tool.\n");
                printf("Please refer to the instruction document for download and usage steps.\n");
                break;
            case 8: exp8_threads();            break;
            case 0: printf("Exiting...\n");    break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}
