#include "dumb_common.h"

#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_FDS 65536

void* thread_worker(void* arg) {
    atomic_fetch_add(&active_threads, 1);
    atomic_fetch_add(&active_connections, 1);

    int client_fd = *(int*)arg;
    free(arg);

    handle_client(client_fd);

    atomic_fetch_sub(&active_connections, 1);
    atomic_fetch_sub(&active_threads, 1);
    return NULL;
}

int main() {
    signal(SIGPIPE, SIG_IGN);

    pthread_t metrics_tid;
    pthread_create(&metrics_tid, NULL, metrics_server_thread, NULL);

    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    int opt = 1;
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt() failed");
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return EXIT_FAILURE;
    }

    if (listen(listener, MAX_FDS) < 0) {
        perror("listen");
        return EXIT_FAILURE;
    }

    printf("DUMB Multithreaded Server listening on 0.0.0.0:%d\n", PORT);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);

        int client_fd = accept(listener, (struct sockaddr*)&client_addr, &addrlen);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        int *fd_ptr = malloc(sizeof(int));
        if (fd_ptr == NULL) {
            perror("malloc");
            close(client_fd);
            continue;
        }

        *fd_ptr = client_fd;

        pthread_t thread_id;
        if (pthread_create(&thread_id, &attr, thread_worker, fd_ptr) != 0) {
            perror("pthread_create");
            free(fd_ptr);
            close(client_fd);
            continue;
        }
    }

    close(listener);

    pthread_join(metrics_tid, NULL);
    return EXIT_SUCCESS;
}
