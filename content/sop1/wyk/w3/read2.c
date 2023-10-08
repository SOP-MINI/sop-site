/* read2.c - the program demonstrates, that file descriptors, which
 * are obtained by two identical open() calls, are related to two
 * file sessions - with separate "current position in the file" counters.
 */

#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
    char c;
    int fd1, fd2;
    struct timespec ts = {0, 50000000};

    if ((fd1 = open("read2.c", O_RDONLY)) < 0)
    {
        perror("open");
        return 1;
    }
    if ((fd2 = open("read2.c", O_RDONLY)) < 0)
    {
        perror("open");
        return 1;
    }
    while (1)
    {
        if (read(fd1, &c, 1) < 1)
            return 0;
        if (write(1, &c, 1) != 1)
        {
            perror("write 1");
            return 2;
        }
        if (read(fd2, &c, 1) < 1)
            return 0;
        if (write(1, &c, 1) != 1)
        {
            perror("write 2");
            return 3;
        }
        nanosleep(&ts, NULL);
    }
    return 0;
}
