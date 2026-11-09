#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/timerfd.h>
#include <stdint.h>

int main() {

    // Create timer
    int timer_fd = timerfd_create(CLOCK_REALTIME, /*TFD_NONBLOCK*/0);
    if (timer_fd == -1) {
        perror("timerfd_create");
        exit(EXIT_FAILURE);
    }

    // Arm timer
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
    if (timerfd_settime(timer_fd, 0, &its, NULL) == -1) {
        perror("timerfd_settime");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 10; i++) {
        uint64_t expirations;
        ssize_t s = read(timer_fd, &expirations, sizeof(expirations));

        if (s != sizeof(uint64_t)) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // if (i == 3) sleep(3);

        printf("Timer tick! (expirations = %lu)\n", expirations);
    }

    close(timer_fd);
    printf("Timer zamknięty. Koniec programu.\n");

    return 0;
}