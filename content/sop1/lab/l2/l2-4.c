#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <linux/input.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

#define MAX_DEVS 32

int is_event_name(const char *name)
{
    if (strncmp(name, "event", 5) != 0)
        return 0;
    for (int i = 5; name[i] != '\0'; i++)
    {
        if (!isdigit(name[i]))
            return 0;
    }
    return 1;
}

int get_descriptors(struct pollfd fds[MAX_DEVS], char paths[MAX_DEVS][256])
{
    int n = 0;
    DIR *dir = opendir("/dev/input");
    if (!dir)
    {
        ERR("opendir");
    }

    struct dirent *de;
    while ((de = readdir(dir)) != NULL)
    {
        if (!is_event_name(de->d_name))
            continue;
        if (n >= MAX_DEVS)
            break;

        snprintf(paths[n], sizeof(paths[n]), "/dev/input/%s", de->d_name);

        int fd = open(paths[n], O_RDONLY | O_NONBLOCK);
        if (fd < 0)
            ERR("open");

        fds[n].fd = fd;
        fds[n].events = POLLIN;
        fds[n].revents = 0;
        n++;
    }
    closedir(dir);
    return n;
}

int main(void)
{
    struct pollfd fds[MAX_DEVS];
    char paths[MAX_DEVS][256];
    int n = get_descriptors(fds, paths);

    if (n == 0)
    {
        fprintf(stderr, "No readable event devices (check permissions)\n");
        return 1;
    }

    for (int i = 0; i < 30; i++)
    {
        int ret = poll(fds, n, -1);
        if (ret < 0)
            ERR("poll");

        for (int i = 0; i < n; i++)
        {
            if (fds[i].fd < 0)
                continue;

            if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
            {
                close(fds[i].fd);
                fds[i].fd = -1;
                continue;
            }

            if (!(fds[i].revents & POLLIN))
                continue;

            struct input_event ev;
            ssize_t r;
            while ((r = read(fds[i].fd, &ev, sizeof(ev))) == sizeof(ev))
            {
                printf("%s: %ld.%06ld type=%u code=%u value=%d\n", paths[i], (long)ev.input_event_sec,
                       (long)ev.input_event_usec, ev.type, ev.code, ev.value);
            }
        }
    }

    for (int i = 0; i < n; i++)
        if (fds[i].fd >= 0)
            close(fds[i].fd);

    return 0;
}
