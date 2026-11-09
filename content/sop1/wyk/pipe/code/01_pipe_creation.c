/**
 * \brief Pipe creation example.
 *
 * Single process app creates pipe and communicates through it with itself.
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

    if (write(fd[1], "text", 5) < 0) {
        perror("write(): ");
        return 1;
    }

    char buf[3];

    ssize_t ret = read(fd[0], buf, sizeof(buf));
    if (ret < 0) {
        perror("write(): ");
        return 1;
    }

    printf("read() returned %ld bytes: '%s'\n", ret, buf);

    ret = read(fd[0], buf, sizeof(buf));
    if (ret < 0) {
        perror("write(): ");
        return 1;
    }

    printf("read() returned %ld bytes: '%s'\n", ret, buf);

    ret = read(fd[0], buf, sizeof(buf));
    if (ret < 0) {
        perror("write(): ");
        return 1;
    }

    printf("read() returned %ld bytes: '%s'\n", ret, buf);

    close(fd[0]);
    close(fd[1]);

    return 0;
}

