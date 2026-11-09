/**
 * \brief Pipe capacity example.
 *
 * Process creates a pipe and fills it with data byte by byte.
 * Observe block on write() call after filling whole pipe.
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>

int main() {

    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe(): ");
        return 1;
    }

    // fd[0] represents read-end of the pipe
    // fd[1] represents write-end of the pipe

    size_t total = 0;
    while(1) {
        ssize_t ret = write(fd[1], "x", 1);
        if (ret < 0) {
            perror("write(): ");
            return 1;
        }
        total += ret;
        printf("Total bytes written: %ld\n", total);
    }
}

