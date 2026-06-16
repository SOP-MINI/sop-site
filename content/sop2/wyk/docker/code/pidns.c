#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define STACK_SIZE (1024 * 1024)

int child_work(void *arg) {
    pid_t parent_pid = *(pid_t*)arg;
    printf("[child] hello!\n");
    printf("[child] pid = %d, ppid = %d!\n", getpid(), getppid());
    printf("[child] real ppid = %d\n", parent_pid);
    sleep(1);

    for (int i = 0; i < 3; i++)
    {
        pid_t pid;
        switch (pid = fork())
        {
            case -1:
                perror("fork()");
                kill(0, SIGKILL);
                exit(EXIT_FAILURE);
            case 0:
                printf("[grandchild %d] pid = %d, ppid = %d\n", i, getpid(), getppid());
                sleep(3);
                printf("[grandchild %d] bye!\n", i);
                exit(EXIT_SUCCESS);
            default:
                printf("[child] created child %d pid = %d\n", i, pid);
                break;
        }
    }

    while (wait(NULL) > 0);

    printf("[child] bye!\n");
    return -1;
}

int main() {
    pid_t parent_pid = getpid();
    printf("[parent] pid = %d\n", parent_pid);

    // Allocate memory for the child's stack
    char *child_stack = malloc(STACK_SIZE);
    if (child_stack == NULL) {
        perror("[HOST] Stack allocation failed");
        exit(EXIT_FAILURE);
    }

    // clone() creates a new process and requires explicit control over what gets inherited.
    // Here we use the CLONE_NEWPID flag to isolate the process tree (create new PID namespace).
    // SIGCHLD ensures the parent gets a signal when the child dies.
    // VM by default gets copied but child process stack starts fresh.
    // In Linux, the stack grows downwards, so we pass a pointer to the end of the buffer
    pid_t child_pid = clone(child_work, child_stack + STACK_SIZE, CLONE_NEWPID | SIGCHLD, &parent_pid);

    if (child_pid == -1) {
        perror("clone()");
        exit(EXIT_FAILURE);
    }

    printf("[parent] child pid = %d\n", child_pid);
    waitpid(child_pid, NULL, 0);
    printf("[parent] child done!\n");
    free(child_stack);

    return 0;
}