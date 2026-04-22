#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
 
int main() {
    pid_t pid;
    pid = fork();
 
    if (pid < 0) {
        fprintf(stderr, "Child is not yet created.");
        return 1;
    }
    else if (pid == 0) {
        printf("Here we are in the child process.");
        printf("[Child] my PID: %d\n", getpid());
        printf("[Child] my parent's PID: %d\n", getppid());
        sleep(2);
        printf("[Child] work done, exiting...\n");
        exit(0);
    }
    else {
        printf("\n[Parent] Hello from the parent process.\n");
        printf("[Parent] My PID: %d\n", getpid());
        printf("[Parent] My child's PID: %d\n", pid);
        printf("[Parent] Waiting for child (%d) to terminate...\n", pid);
 
        int status;
        waitpid(pid, &status, 0);
 
        if (WIFEXITED(status)) {
            printf("[Parent] Child terminated normally with status: %d\n", WEXITSTATUS(status));
        }
    }
 
    return 0;
}
 
 gcc vishal1.c
 gcc -o vishal vishal1.c
 ./vishal
