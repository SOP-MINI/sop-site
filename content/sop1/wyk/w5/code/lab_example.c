#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void sigusr1_handler(int i) {
    printf("SIGUSR1 handler in %d\n", getpid());
}

void sigusr2_handler(int i) {
    printf("SIGUSR2 handler in %d\n", getpid());
}

volatile sig_atomic_t done = 0;

void sigint_handler(int i) {
    printf("SIGINT handler in %d\n", getpid());
    done = 1;
}

int child_work(int i, sigset_t *oldmask) {
    int counter = 0;
    while (!done) {
        sigsuspend(oldmask);
        if (done) {
            printf("child %d exits\n", getpid());
            break;
        }
        printf("child %d starts working!\n", i);
        counter++;
        sleep(1);
        printf("child %d ends work!\n", i);

        // send process back
        if (kill(getppid(), SIGUSR2) < 0) {
            perror("kill");
            break;
        }
    }

    if (counter > 100) {
        printf("truncating counter value %d\n", counter);
        counter = 100;
    }

    return counter;
}

void parent_work(int n, pid_t *child_pids, sigset_t *oldmask) {
    srand(getpid());

    int run = 1;
    while (!done) {
        sleep(3);
        int i = rand() % n;
        printf("==========> sending SIGUSR1 to child %d (pid = %d) <===========\n", i, child_pids[i]);
        if (kill(child_pids[i], SIGUSR1) < 0) {
            perror("kill");
            kill(0, SIGKILL);
            return;
        }

        printf("parent waiting for SIGUSR2\n");
        sigsuspend(oldmask);
        printf("parent received SIGUSR2\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);

    pid_t *child_pids = malloc(n * sizeof(pid_t));

    struct sigaction sigusr1_action;
    memset(&sigusr1_action, 0, sizeof(sigusr1_action));
    sigusr1_action.sa_handler = sigusr1_handler;
    if (sigaction(SIGUSR1, &sigusr1_action, NULL) < 0) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    struct sigaction sigusr2_action;
    memset(&sigusr2_action, 0, sizeof(sigusr2_action));
    sigusr2_action.sa_handler = sigusr2_handler;
    if (sigaction(SIGUSR2, &sigusr2_action, NULL) < 0) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    struct sigaction sigint_action;
    memset(&sigint_action, 0, sizeof(sigint_action));
    sigint_action.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &sigint_action, NULL) < 0) {
        perror("sigaction");
        return EXIT_FAILURE;
    }


    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        switch (pid) {
            case -1:
                perror("fork");
                kill(0, SIGKILL);
                return EXIT_FAILURE;
            case 0:
                exit(child_work(i, &oldmask));
            default:
                printf("created child %d\n", pid);
                child_pids[i] = pid;
                break;
        }
    }

    parent_work(n, child_pids, &oldmask);

    for (int i = 0; i < n; i++) {
        int status;
        pid_t pid = wait(&status);
        if (WIFEXITED(status)) {
            printf("waited child %d - worked %d times\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("waited child %d - killed by signal %d\n", pid, WTERMSIG(status));
        }

    }

    free(child_pids);

    return EXIT_SUCCESS;
}
