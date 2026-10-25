#define _POSIX_C_SOURCE 200809L

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timerfd.h>
#include <unistd.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

int main(void)
{
    struct itimerspec new_value;
    memset(&new_value, 0, sizeof(new_value));
    new_value.it_value.tv_sec = 2;

    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd == -1)
        ERR("timerfd_create");

    printf("Preparing timer for waiting for 2 seconds\n");
    if (timerfd_settime(fd, 0, &new_value, NULL) == -1)
        ERR("timerfd_settime");

    uint64_t val = 0;
    if (read(fd, &val, sizeof(uint64_t)) < 0)
        ERR("read");
    printf("2 seconds passed, got %lu from timer, exiting now\n", val);

    return 0;
}
