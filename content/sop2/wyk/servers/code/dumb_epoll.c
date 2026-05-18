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
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

#define MAX_EVENTS 10000
#define MAX_FDS 65536

// State machine states
typedef enum {
    STATE_READ_COMMAND,
    STATE_WRITE_STREAM,
    STATE_WRITE_PONG,
    STATE_WRITE_BURN,
    STATE_WRITE_DONE
} client_state_t;

// Single client context
typedef struct {
    int fd;
    client_state_t state;

    // Read I/O context
    char read_buf[1024];
    size_t read_len;

    // Write I/O context
    long bytes_to_send;
    long bytes_sent;
} client_ctx_t;

// Global context table indexed by fd
client_ctx_t clients[MAX_FDS];

static int make_socket_non_blocking(int sfd) {
    int flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) return -1;
    flags |= O_NONBLOCK;
    if (fcntl(sfd, F_SETFL, flags) == -1) return -1;
    return 0;
}

void init_client_ctx(int fd) {
    clients[fd].fd = fd;
    clients[fd].state = STATE_READ_COMMAND;
    clients[fd].read_len = 0;
    clients[fd].bytes_to_send = 0;
    clients[fd].bytes_sent = 0;
}

void modify_epoll_state(int epollfd, int fd, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

// Read I/O event handler
int handle_read(int epollfd, int client_fd) {
    client_ctx_t *ctx = &clients[client_fd];

    ssize_t count = read(client_fd, ctx->read_buf + ctx->read_len, sizeof(ctx->read_buf) - 1 - ctx->read_len);

    if (count == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) return 1;
        return 0;
    } else if (count == 0) {
        return 0;
    }

    ctx->read_len += count;
    ctx->read_buf[ctx->read_len] = '\0';

    // Try to parse a command
    char *newline = strchr(ctx->read_buf, '\n');
    if (newline) {
        *newline = '\0';

        if (strncmp(ctx->read_buf, "PING", 4) == 0) {
            atomic_fetch_add(&total_ping_requests, 1);
            ctx->state = STATE_WRITE_PONG;
            ctx->bytes_sent = 0;
            ctx->bytes_to_send = 5; // "PONG\n"
        }
        else if (strncmp(ctx->read_buf, "BURN", 4) == 0) {
            atomic_fetch_add(&total_burn_requests, 1);

            long long depth = 0;
            sscanf(ctx->read_buf, "BURN %lld", &depth);

            long long sum = recursive_burn(depth);

            ctx->state = STATE_WRITE_BURN;
            ctx->bytes_sent = 0;
            ctx->bytes_to_send = snprintf(ctx->read_buf, sizeof(ctx->read_buf), "RESULT %lld\n", sum);
        }
        else if (strncmp(ctx->read_buf, "STREAM", 6) == 0) {
            atomic_fetch_add(&total_stream_requests, 1);
            long bytes = 65536;
            sscanf(ctx->read_buf, "STREAM %ld", &bytes);
            ctx->state = STATE_WRITE_STREAM;
            ctx->bytes_sent = 0;
            ctx->bytes_to_send = bytes;
        }
        else {
            atomic_fetch_add(&total_unknown_requests, 1);
            ctx->read_len = 0; // Ignore and reset
            return 1;
        }

        // Now wait for write-ready event
        modify_epoll_state(epollfd, client_fd, EPOLLOUT);
    } else if (ctx->read_len == sizeof(ctx->read_buf) - 1) {
        // Overflow protection
        ctx->read_len = 0;
    }

    return 1;
}

// Write I/O event handler
int handle_write(int epollfd, int client_fd) {
    client_ctx_t *ctx = &clients[client_fd];

    if (ctx->state == STATE_WRITE_PONG) {
        char *msg = "PONG\n";
        long remaining = ctx->bytes_to_send - ctx->bytes_sent;

        ssize_t written = write(client_fd, msg + ctx->bytes_sent, remaining);
        if (written < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) return 1;
            return 0; // Błąd sieciowy
        }
        ctx->bytes_sent += written;

        if (ctx->bytes_sent == ctx->bytes_to_send) {
            // Back to reading
            ctx->state = STATE_READ_COMMAND;
            ctx->read_len = 0;
            modify_epoll_state(epollfd, client_fd, EPOLLIN);
        }
    }
    else if (ctx->state == STATE_WRITE_BURN) {
        long remaining = ctx->bytes_to_send - ctx->bytes_sent;

        ssize_t written = write(client_fd, ctx->read_buf + ctx->bytes_sent, remaining);
        if (written < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) return 1;
            return 0;
        }
        ctx->bytes_sent += written;

        if (ctx->bytes_sent == ctx->bytes_to_send) {
            // Back to reading
            ctx->state = STATE_READ_COMMAND;
            ctx->read_len = 0;
            modify_epoll_state(epollfd, client_fd, EPOLLIN);
        }
    }
    else if (ctx->state == STATE_WRITE_STREAM) {
        char chunk[16384];
        memset(chunk, 'A', sizeof(chunk));

        long remaining = ctx->bytes_to_send - ctx->bytes_sent;
        size_t to_write = (remaining < sizeof(chunk)) ? remaining : sizeof(chunk);

        ssize_t written = write(client_fd, chunk, to_write);
        if (written < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) return 1;
            return 0;
        }
        ctx->bytes_sent += written;

        if (ctx->bytes_sent == ctx->bytes_to_send) {
            // Stream written, now write "DONE\n" trailer
            ctx->state = STATE_WRITE_DONE;
            ctx->bytes_sent = 0;
            ctx->bytes_to_send = 5; // "DONE\n"
        }
    }
    else if (ctx->state == STATE_WRITE_DONE) {
        char *msg = "DONE\n";
        long remaining = ctx->bytes_to_send - ctx->bytes_sent;

        ssize_t written = write(client_fd, msg + ctx->bytes_sent, remaining);
        if (written < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) return 1;
            return 0;
        }
        ctx->bytes_sent += written;

        if (ctx->bytes_sent == ctx->bytes_to_send) {
            // Back to reading
            ctx->state = STATE_READ_COMMAND;
            ctx->read_len = 0;
            modify_epoll_state(epollfd, client_fd, EPOLLIN);
        }
    }

    return 1;
}

int main() {
    signal(SIGPIPE, SIG_IGN);

    active_threads = 1;

    pthread_t metrics_tid;
    pthread_create(&metrics_tid, NULL, metrics_server_thread, NULL);

    int listener = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    make_socket_non_blocking(listener);

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

    printf("DUMB epoll server listening on 0.0.0.0:%d\n", PORT);

    int epollfd = epoll_create1(0);
    struct epoll_event ev, events[MAX_EVENTS];

    ev.events = EPOLLIN;
    ev.data.fd = listener;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listener, &ev);

    while (1) {
        int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);

        for (int i = 0; i < nfds; i++) {
            int current_fd = events[i].data.fd;

            if (current_fd == listener) {
                // New connection
                while (1) {
                    struct sockaddr_in client_addr;
                    socklen_t addrlen = sizeof(client_addr);
                    int client_fd = accept(listener, (struct sockaddr*)&client_addr, &addrlen);

                    if (client_fd == -1) break;
                    if (client_fd >= MAX_FDS) {
                        close(client_fd);
                        continue;
                    }

                    make_socket_non_blocking(client_fd);
                    init_client_ctx(client_fd);

                    ev.events = EPOLLIN;
                    ev.data.fd = client_fd;
                    epoll_ctl(epollfd, EPOLL_CTL_ADD, client_fd, &ev);

                    atomic_fetch_add(&active_connections, 1);
                }
            } else {
                // Existing client
                int is_alive = 1;

                if (events[i].events & EPOLLIN) {
                    is_alive = handle_read(epollfd, current_fd);
                }
                else if (events[i].events & EPOLLOUT) {
                    is_alive = handle_write(epollfd, current_fd);
                }

                if (!is_alive || (events[i].events & (EPOLLERR | EPOLLHUP))) {
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, current_fd, NULL);
                    close(current_fd);
                    atomic_fetch_sub(&active_connections, 1);
                }
            }
        }
    }

    close(listener);
    close(epollfd);
    return 0;
}