#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
    {
        perror("sigprocmask");
        return 1;
    }

    // Map anonymous shared memory
    size_t length = 4096;
    char* shared_mem = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (shared_mem == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    printf("[%d] Memory mapped.\n", getpid());

    // Create a child process
    pid_t pid = fork();

    switch (pid)
    {
        case -1:
            perror("fork");
            return 1;
        case 0:
        {
            printf("[%d] Started. Waiting for SIGUSR1 from parent...\n", getpid());

            int received_sig;
            // sigwait() will suspend the child until SIGUSR1 is pending
            if (sigwait(&set, &received_sig) != 0)
            {
                perror("sigwait");
                exit(1);
            }

            printf("[%d] Received signal %d! Reading memory...\n", getpid(), received_sig);
            printf("[%d] Data in memory: '%s'\n", getpid(), shared_mem);

            munmap(shared_mem, length);
            printf("[%d] Exiting.\n", getpid());
            exit(0);
        }
        default:
            break;
    }

    printf("[%d] Doing some work (simulated delay)...\n", getpid());
    sleep(2);  // Simulate heavy computation before providing data

    printf("[%d] Writing result to shared memory...\n", getpid());
    strcpy(shared_mem, "I truly do love OPS2");

    printf("[%d] Sending SIGUSR1 to child %d...\n", getpid(), pid);
    kill(pid, SIGUSR1);  // Notify the child

    wait(NULL);  // Wait for the child to safely terminate
    printf("[%d] Child finished. Cleaning up.\n", getpid());
    munmap(shared_mem, length);

    return 0;
}
