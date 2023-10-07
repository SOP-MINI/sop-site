/* read_fork.c - the program demonstrates, that file descriptor, which
 * is inherited by a child process is related to the same file sessions
 * as that o parent. In effect operations on each of
 * file descriptors affect common "current position in the file" counter.
 */

#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
    char c;
    int fd;
    struct timespec ts = {1, 50000000};

    if ((fd = open("read_fork.c", O_RDONLY)) < 0)
    {
        perror("open");
        return 1;
    }
    if (fork() < 0)
    {
        perror("fork");
        return 1;
    }
    while (1)
    {
        if (read(fd, &c, 1) < 1)
            return 0;
        if (write(1, &c, 1) != 1)
        {
            perror("write 1");
            return 2;
        }
        fsync(1);
        nanosleep(&ts, NULL);
    }
    return 0;
}
