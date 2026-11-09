#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/inotify.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <dir path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct stat s;
    if (stat(argv[1], &s) < 0) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    if (!(s.st_mode & S_IFDIR)) {
        fprintf(stderr, "not a directory: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Create inotify fd
    int notify_fd = inotify_init();
    if (notify_fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    int watch_fd = inotify_add_watch(notify_fd, argv[1],
                                 IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM);

    if (watch_fd == -1) {
        fprintf(stderr, "failed to add watch on %s\n", argv[1]);
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    printf("watching: %s\n", argv[1]);

    int run = 1;
    while (run) {
        char buffer[(sizeof(struct inotify_event) + NAME_MAX) * 10];
        ssize_t len = read(notify_fd, buffer, sizeof(buffer));
        if (len < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        ssize_t i = 0;
        while (i < len) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            printf("[i=%ld] [ev=0x%08x]", i, event->mask);

            if (event->mask & IN_CREATE) {
                printf("'%s' was CREATED", event->name);
            }
            if (event->mask & IN_DELETE) {
                printf("'%s' was DELETED", event->name);
            }
            if (event->mask & IN_MODIFY) {
                printf("'%s' was MODIFIED", event->name);
            }
            if (event->mask & IN_MOVED_FROM) {
                printf("'%s' was MOVED FROM", event->name);
            }
            if (event->mask & IN_MOVED_TO) {
                printf("'%s' was MOVED TO", event->name);
            }

            if (event->mask & IN_IGNORED) {
                printf("Watch was removed - exiting\n");
                run = 0;
                break;
            }

            printf("\n");

            i += sizeof(struct inotify_event) + event->len;
        }
    }

    inotify_rm_watch(notify_fd, watch_fd);
    close(notify_fd);

    return 0;
}