#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/timerfd.h>
#include <sys/signalfd.h>
#include <sys/select.h>

int max(int a, int b, int c) {
    int m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    return m;
}

int setup_signal_fd() {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    int sfd = signalfd(-1, &mask, 0);
    if (sfd == -1) {
        perror("signalfd");
        exit(EXIT_FAILURE);
    }
    return sfd;
}

int setup_timer_fd() {
    int tfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (tfd == -1) {
        perror("timerfd_create");
        exit(EXIT_FAILURE);
    }

    struct itimerspec its;
    its.it_value.tv_sec = 3;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 3;
    its.it_interval.tv_nsec = 0;

    if (timerfd_settime(tfd, 0, &its, NULL) == -1) {
        perror("timerfd_settime");
        exit(EXIT_FAILURE);
    }
    return tfd;
}

int main() {
    int signal_fd = setup_signal_fd();
    int timer_fd = setup_timer_fd();
    int stdin_fd = STDIN_FILENO;

    // We must make stdin non-blocking
    // Or a read() could block our whole loop
    fcntl(stdin_fd, F_SETFL, O_NONBLOCK);

    fd_set read_fds; // The set of file descriptors to watch
    int max_fd = max(signal_fd, timer_fd, stdin_fd);

    // Event loop
    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(stdin_fd, &read_fds);
        FD_SET(signal_fd, &read_fds);
        FD_SET(timer_fd, &read_fds);

        printf("calling select()\n-> ");
        fflush(stdout);

        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        printf("returned %d\n", activity);
        if (activity < 0) {
            perror("select");
            break;
        }

        if (FD_ISSET(signal_fd, &read_fds)) {
            struct signalfd_siginfo fdsi;
            read(signal_fd, &fdsi, sizeof(fdsi));
            printf("received C-c, exiting!\n");
            break;
        }

        if (FD_ISSET(timer_fd, &read_fds)) {
            uint64_t expirations;
            read(timer_fd, &expirations, sizeof(expirations));
            printf("received timer tick (%ld)\n", expirations);
        }

        if (FD_ISSET(stdin_fd, &read_fds)) {
            char line[1024];
            ssize_t bytes_read = read(stdin_fd, line, sizeof(line) - 1);

            if (bytes_read == 0) {
                printf("received stdin EOF, exiting!\n");
                break;
            }

            if (bytes_read > 0) {
                line[bytes_read] = '\0';
                line[strcspn(line, "\n")] = 0;
                printf("received command '%s'\n", line);
            }
        }
    }


    printf("event loop finished\n");

    close(signal_fd);
    close(timer_fd);

    return 0;
}