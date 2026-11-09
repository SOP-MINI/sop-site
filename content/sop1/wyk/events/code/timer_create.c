#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SIG SIGUSR1

volatile sig_atomic_t counter = 0;

void timer_handler(int sig) {
    counter++;
    printf("Timer tick! (sig = %d, counter = %d)\n", sig, counter);
}

void print_time() {
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) < 0) {
        perror("clock_gettime");
        return;
    }
    printf("ts=%ld.%09ld\n", ts.tv_sec, ts.tv_nsec);
}

int main() {
    // Setup signal handler h
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = 0;
    sa.sa_handler = timer_handler;
    if (sigaction(SIG, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Configure notification by signal
    struct sigevent sev;
    memset(&sev, 0, sizeof(sev));
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;

    // Create timer
    timer_t timerid;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    printf("New timer ID: %ld\n", (long) timerid);

    // Arm timer to start ticking 2s from now, with 1s interval
    struct itimerspec its = {
        .it_value = {
            .tv_sec = 2,
            .tv_nsec = 0
        },
        .it_interval = {
            .tv_sec = 1,
            .tv_nsec = 0
        }
    };

    if (timer_settime(timerid, 0, &its, NULL) == -1) {
         perror("timer_settime");
         exit(EXIT_FAILURE);
    }

    sigset_t sigset, old;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIG);
    if (sigprocmask(SIG_BLOCK, &sigset, &old) < 0) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    print_time();
    while (counter < 10) {
        sigsuspend(&old);

        print_time();
    }

    sigprocmask(SIG_SETMASK, &old, NULL);

    timer_delete(timerid);

    return 0;
}