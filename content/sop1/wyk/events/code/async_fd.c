#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void sigio_handler(int signum) {
    char msg[] = "\n[SIGIO Handler] Data is ready on STDIN!\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);

    char buffer[100];
    int bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer)-1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        char output_msg[150];
        int len = snprintf(output_msg, sizeof(output_msg), "Read: %s", buffer);
        write(STDOUT_FILENO, output_msg, len);
    }
}

int main() {
    struct sigaction sa;
    int flags;

    printf("Setting up Async I/O on STDIN. PID: %d\n", getpid());

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigio_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGIO, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    if (fcntl(STDIN_FILENO, F_SETOWN, getpid()) == -1) {
        perror("fcntl F_SETOWN");
        exit(EXIT_FAILURE);
    }

    flags = fcntl(STDIN_FILENO, F_GETFL);
    if (fcntl(STDIN_FILENO, F_SETFL, flags | O_ASYNC | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }

    printf("Configuration complete. Waiting... (Type something and hit Enter)\n");

    // Main loop does "other work"
    while (1) {
        write(STDOUT_FILENO, ".", 1);
        sleep(1);
    }

    return 0;
}