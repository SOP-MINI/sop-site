/**
 * \brief Unhandled EPIPE error example.
 *
 * Process creates pipe, closes it's read-end and tries to write to it.
 * In response system sends SIGPIPE signal to the process.
 */

#include <unistd.h>
#include <stdio.h>

int main() {

    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe(): ");
        return 1;
    }

    // fd[0] represents read-end of the pipe
    // fd[1] represents write-end of the pipe

    close(fd[0]); // close read-end

    if (write(fd[1], "text", 5) < 0) {
        perror("write(): ");
        return 1;
    }

    close(fd[1]);

    return 0;
}
