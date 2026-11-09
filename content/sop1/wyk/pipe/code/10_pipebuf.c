/**
 * \brief PIPE_BUF behavior example.
 *
 * Process creates a pipe and fills it with data leaving PIPE_BUF / 2 free space.
 * Then it attempts overflow the pipe with next write call.
 * Observe different behavior depending on blocking/nonblocking mode and/or last write size.
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>

int set_nonblock(int desc) {
    int oldflags = fcntl(desc, F_GETFL, 0);
    if (oldflags == -1)
        return -1;
    oldflags |= O_NONBLOCK;
    return fcntl(desc, F_SETFL, oldflags);
}

// Important: This example assumes 64kB pipe capacity - it may need adjustment on your system
#define PIPE_CAP 65536

int main() {

    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe(): ");
        return 1;
    }

    set_nonblock(fd[1]); // TODO: Uncomment and experiment with nonblocking write mode

    char buf[PIPE_BUF + 50];
    memset(buf, 0, sizeof(buf));

    // Leave PIPE_BUF / 2 free space in pipe
    size_t total = 0;
    for (int i = 0; i < PIPE_CAP / ( PIPE_BUF / 2 ) - 1; ++i) {
        ssize_t ret = write(fd[1], buf, PIPE_BUF / 2);
        if (ret < 0) {
            perror("write(): ");
            return 1;
        }
        total += ret;
        printf("Total bytes written: %ld\n", total);
    }

    printf("Trying to write more bytes\n");
    ssize_t ret = write(fd[1], buf, PIPE_BUF + 50); // TODO: Check what happens when trying to write 1 byte more
    if (ret < 0) {
        perror("write(): ");
        return 1;
    }
    printf("Done! Written %ld bytes\n", ret);

    return 0;
}
