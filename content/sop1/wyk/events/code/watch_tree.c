#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <limits.h>

#define MAX_WATCHES 8192
#define EVENT_BUF_LEN (64 * (sizeof(struct inotify_event) + NAME_MAX + 1))

struct Watch {
    int wd; // key
    char *path; // value
};

struct WatchMap {
    struct Watch watch_map[MAX_WATCHES];
    int watch_count;
};

void add_to_map(struct WatchMap *map, int wd, const char *path) {
    if (map->watch_count >= MAX_WATCHES) {
        fprintf(stderr, "Exceeded max watches!\n");
        return;
    }
    map->watch_map[map->watch_count].wd = wd;
    map->watch_map[map->watch_count].path = strdup(path); // Must copy the path!
    map->watch_count++;
    printf("new watch: '%s' @wd=%d\n", path, wd);
}

struct Watch *find_watch(struct WatchMap *map, int wd) {
    for (int i = 0; i < map->watch_count; i++) {
        if (map->watch_map[i].wd == wd) {
            return &map->watch_map[i];
        }
    }
    return NULL;
}

void remove_from_map(struct WatchMap *map, int wd) {
    for (int i = 0; i < map->watch_count; i++) {
        if (map->watch_map[i].wd == wd) {
            printf("removing watch: '%s' @wd=%d\n", map->watch_map[i].path, wd);
            free(map->watch_map[i].path);
            map->watch_map[i] = map->watch_map[map->watch_count - 1];
            map->watch_count--;
            return;
        }
    }
}

void add_watch_recursive(int notify_fd, struct WatchMap *map, const char *base_path) {
    uint32_t mask = IN_CREATE | IN_DELETE | IN_MODIFY |
                    IN_MOVED_FROM | IN_MOVED_TO;

    int wd = inotify_add_watch(notify_fd, base_path, mask);
    if (wd < 0) {
        perror("inotify_add_watch");
        return;
    }
    add_to_map(map, wd, base_path);

    DIR *dir = opendir(base_path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode)) {
            add_watch_recursive(notify_fd, map, full_path);
        }
    }

    closedir(dir);
}

void update_watch_paths(struct WatchMap *map, const char *old_path, const char *new_path) {
    size_t old_len = strlen(old_path);

    for (int i = 0; i < map->watch_count; i++) {
        if (strncmp(map->watch_map[i].path, old_path, old_len) == 0 && (
                map->watch_map[i].path[old_len] == '/' || map->watch_map[i].path[old_len] == '\0')) {
            char new_full_path[PATH_MAX];
            // Sklej nową ścieżkę z resztą (sufiksem) starej ścieżki
            snprintf(new_full_path, sizeof(new_full_path), "%s%s",
                     new_path, map->watch_map[i].path + old_len);

            free(map->watch_map[i].path);
            map->watch_map[i].path = strdup(new_full_path);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_to_watch>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int notify_fd = inotify_init();
    if (notify_fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    struct WatchMap map = {0};

    printf("initial scan...\n");
    add_watch_recursive(notify_fd, &map, argv[1]);
    printf("done!\n");

    uint32_t pending_cookie = 0;
    char pending_move_path[PATH_MAX] = "";

    while (map.watch_count > 0) {
        char buffer[EVENT_BUF_LEN];
        ssize_t len = read(notify_fd, buffer, EVENT_BUF_LEN);
        if (len < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        ssize_t i = 0;
        while (i < len) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            printf("[i=%ld] [ev=0x%08x] [wd=%d] ", i, event->mask, event->wd);

            struct Watch *watch = find_watch(&map, event->wd);

            char event_path[PATH_MAX] = "";
            if (watch && event->len > 0) {
                snprintf(event_path, sizeof(event_path), "%s/%s", watch->path, event->name);
            } else if (watch) {
                strncpy(event_path, watch->path, sizeof(event_path));
            }

            if (event->mask & IN_IGNORED) {
                // Watch was removed by the kernel
                remove_from_map(&map, event->wd);
            } else if (event->mask & IN_ISDIR) {
                printf("Directory '%s' was ", event_path);

                if (event->mask & IN_CREATE) {
                    printf("CREATED");
                    add_watch_recursive(notify_fd, &map, event_path);
                } if (event->mask & IN_DELETE) {
                    printf("DELETED");
                    // No need to handle, IN_IGNORED will do
                } else if (event->mask & IN_MOVED_FROM) {
                    printf("MOVED_FROM (cookie=%u)", event->cookie);
                    pending_cookie = event->cookie;
                    strncpy(pending_move_path, event_path, sizeof(pending_move_path));
                } else if (event->mask & IN_MOVED_TO) {
                    printf("MOVED_TO (cookie=%u)", event->cookie);

                    if (event->cookie == pending_cookie && pending_cookie != 0) {
                        update_watch_paths(&map, pending_move_path, event_path);

                        pending_cookie = 0;
                        pending_move_path[0] = '\0';
                    } else {
                        add_watch_recursive(notify_fd, &map, event_path);
                    }
                }

                printf("\n");
            } else {
                printf("File '%s' was ", event_path);

                if (event->mask & IN_MODIFY) {
                    printf("MODIFIED");
                }
                if (event->mask & IN_CREATE && !(event->mask & IN_ISDIR)) {
                    printf("CREATED");
                }
                if (event->mask & IN_DELETE && !(event->mask & IN_ISDIR)) {
                    printf("DELETED");
                }
                if (event->mask & IN_MOVED_FROM) {
                    printf("MOVED_FROM");
                }
                if (event->mask & IN_MOVED_TO && !(event->mask & IN_ISDIR)) {
                    printf("MOVED_TO");
                }

                printf("\n");
            }

            i += sizeof(struct inotify_event) + event->len;
        }
    }

    // Cleanup (though we'll never reach here in this demo)
    close(notify_fd);
    return 0;
}
