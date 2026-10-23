#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

void set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1)
        ERR("fcntl");
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1)
        ERR("fcntl");
}

int main(void)
{
    char buf[128];
    ssize_t n;

    puts("Testing blocking scanf - type something and press enter:");
    scanf("%100s", buf);
    printf("You typed: %s\n", buf);

    puts("Testing blocking read - type something and press enter:");
    n = read(STDIN_FILENO, buf, sizeof(buf) - 1);
    if (n < 0)
        ERR("read");
    buf[n] = 0;
    printf("Got %lu bytes: \"%s\"\n", n, buf);
    if (buf[n - 1] == '\n')
        puts("(your last character was \\n)");

    set_nonblocking(STDIN_FILENO);
    printf("Nonblocking read for ~10s, type something and press enter\n");

    for (int i = 0; i < 10; i++)
    {
        printf("Try %d/10...\n", i + 1);
        n = read(STDIN_FILENO, buf, sizeof(buf) - 1);
        if (n > 0)
        {
            buf[n] = '\0';
            printf("Got %zd bytes: %s", n, buf);
        }
        else if (n == 0)
        {
            printf("EOF\n");
            break;
        }
        else if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            printf("No data yet (EAGAIN)...\n");
        }
        else
        {
            ERR("read");
        }
        sleep(1);
    }
    return 0;
}
