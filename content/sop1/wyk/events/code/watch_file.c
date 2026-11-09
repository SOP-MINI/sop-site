#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct stat s;
    if (stat(argv[1], &s) < 0) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    if (!(s.st_mode & S_IFREG)) {
        fprintf(stderr, "not a regular file: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    int notify_fd = inotify_init();
    if (notify_fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    int watch_fd = inotify_add_watch(notify_fd, argv[1],
                                 IN_MODIFY | IN_ATTRIB | IN_DELETE_SELF | IN_MOVE_SELF);
    if (watch_fd < 0) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    printf("watching: %s\n", argv[1]);

    int run = 1;
    while (run) {
        char buffer[10 * sizeof(struct inotify_event)];
        ssize_t len = read(notify_fd, buffer, sizeof(buffer));
        if (len < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        ssize_t i = 0;
        while (i < len) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            printf("[i=%ld] [ev=0x%08x] ", i, event->mask);

            if (event->mask & IN_MODIFY) {
                printf("'%s' was MODIFIED", argv[1]);
            }
            if (event->mask & IN_ATTRIB) {
                printf("'%s' has ATTRIB change", argv[1]);
            }
            if (event->mask & IN_DELETE_SELF) {
                printf("'%s' was DELETE_SELF", argv[1]);
            }
            if (event->mask & IN_MOVE_SELF) {
                printf("'%s' was MOVE_SELF", argv[1]);
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

    close(notify_fd);
    return 0;
}