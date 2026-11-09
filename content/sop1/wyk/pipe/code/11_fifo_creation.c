/**
 * \brief FIFO opening example.
 *
 * Process creates FIFO file and tries to open it.
 * Observe different behavior (blocking/error/success) depending on open mode.
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_NAME "my.fifo"

int main() {

    unlink(FIFO_NAME);

    if (mkfifo(FIFO_NAME, 0666) < 0) {
        perror("mkfifo(): ");
        return 1;
    }

    int fd;
    printf("Trying to open FIFO '%s'\n", FIFO_NAME);
    // TODO: Experiment with passing O_WRONLY and/or O_NONBLOCK below
    if ((fd = open(FIFO_NAME, O_RDONLY)) < 0) {
        perror("open(): ");
        return 1;
    }
    printf("Done\n");

    char buf[15];
    int ret = read(fd, buf, 15);
    printf("read() returned %d\n", ret);

    close(fd);

    return 0;
}
