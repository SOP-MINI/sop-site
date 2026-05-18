#include "dumb_common.h"

#define _GNU_SOURCE
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
#include <sched.h>

#define MAX_EVENTS 10000
#define MAX_FDS 65536

typedef enum {
    STATE_READ_COMMAND,
    STATE_WRITE_STREAM,
    STATE_WRITE_PONG,
    STATE_WRITE_BURN,
    STATE_WRITE_DONE
} client_state_t;

typedef struct {
    int fd;
    client_state_t state;
    char read_buf[1024];
    size_t read_len;
    long bytes_to_send;
    long bytes_sent;
} client_ctx_t;

client_ctx_t clients[MAX_FDS];

typedef struct {
    int id;
    int epoll_fd;
    pthread_t thread_id;

    // Independent worker counters
    atomic_ullong active_connections;
    atomic_ullong total_ping_requests;
    atomic_ullong total_burn_requests;
    atomic_ullong total_stream_requests;
    atomic_ullong total_unknown_requests;
} worker_ctx_t;

worker_ctx_t* workers;
long num_cores_global = 0;

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

int handle_read(worker_ctx_t *worker, int client_fd) {
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

    char *newline = strchr(ctx->read_buf, '\n');
    if (newline) {
        *newline = '\0';

        if (strncmp(ctx->read_buf, "PING", 4) == 0) {
            atomic_fetch_add(&worker->total_ping_requests, 1);
            ctx->state = STATE_WRITE_PONG;
            ctx->bytes_sent = 0;
            ctx->bytes_to_send = 5;
        }
        else if (strncmp(ctx->read_buf, "BURN", 4) == 0) {
            atomic_fetch_add(&worker->total_burn_requests, 1);
            long long depth = 0;
            sscanf(ctx->read_buf, "BURN %lld", &depth);
            long long sum = recursive_burn(depth);
            ctx->state = STATE_WRITE_BURN;
            ctx->bytes_sent = 0;
            ctx->bytes_to_send = snprintf(ctx->read_buf, sizeof(ctx->read_buf), "RESULT %lld\n", sum);
        }
        else if (strncmp(ctx->read_buf, "STREAM", 6) == 0) {
            atomic_fetch_add(&worker->total_stream_requests, 1);
            long bytes = 65536;
            sscanf(ctx->read_buf, "STREAM %ld", &bytes);
            ctx->state = STATE_WRITE_STREAM;
            ctx->bytes_sent = 0;
            ctx->bytes_to_send = bytes;
        }
        else {
            atomic_fetch_add(&worker->total_unknown_requests, 1);
            ctx->read_len = 0;
            return 1;
        }
        modify_epoll_state(worker->epoll_fd, client_fd, EPOLLOUT);
    } else if (ctx->read_len == sizeof(ctx->read_buf) - 1) {
        ctx->read_len = 0;
    }
    return 1;
}

int handle_write(worker_ctx_t *worker, int client_fd) {
    client_ctx_t *ctx = &clients[client_fd];

    if (ctx->state == STATE_WRITE_PONG) {
        char *msg = "PONG\n";
        long remaining = ctx->bytes_to_send - ctx->bytes_sent;
        ssize_t written = write(client_fd, msg + ctx->bytes_sent, remaining);
        if (written < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) return 1;
            return 0;
        }
        ctx->bytes_sent += written;

        if (ctx->bytes_sent == ctx->bytes_to_send) {
            ctx->state = STATE_READ_COMMAND;
            ctx->read_len = 0;
            modify_epoll_state(worker->epoll_fd, client_fd, EPOLLIN);
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
            ctx->state = STATE_READ_COMMAND;
            ctx->read_len = 0;
            modify_epoll_state(worker->epoll_fd, client_fd, EPOLLIN);
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
            ctx->state = STATE_WRITE_DONE;
            ctx->bytes_sent = 0;
            ctx->bytes_to_send = 5;
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
            ctx->state = STATE_READ_COMMAND;
            ctx->read_len = 0;
            modify_epoll_state(worker->epoll_fd, client_fd, EPOLLIN);
        }
    }
    return 1;
}

void *worker_metrics_server_thread(void *arg) {
    (void)arg;
    int opt = 1;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9000);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 10);

    while(1) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) continue;

        char dummy[1024];
        ssize_t r = read(client_fd, dummy, sizeof(dummy) - 1);
        if (r <= 0) {
            close(client_fd);
            continue;
        }

        char payload[8192] = "";
        int offset = 0;

        offset += snprintf(payload + offset, sizeof(payload) - offset,
            "# HELP server_active_threads Client threads\n"
            "# TYPE server_active_threads gauge\n"
            "server_active_threads %llu\n",
            atomic_load(&active_threads));

        offset += snprintf(payload + offset, sizeof(payload) - offset,
            "# HELP server_active_connections Active inbound connections\n"
            "# TYPE server_active_connections gauge\n");

        for (int i = 0; i < num_cores_global; i++) {
            offset += snprintf(payload + offset, sizeof(payload) - offset,
                "server_active_connections{worker=\"%d\"} %llu\n",
                workers[i].id, atomic_load(&workers[i].active_connections));
        }

        offset += snprintf(payload + offset, sizeof(payload) - offset,
            "# HELP server_requests_total Received requests\n"
            "# TYPE server_requests_total counter\n");

        for (int i = 0; i < num_cores_global; i++) {
            offset += snprintf(payload + offset, sizeof(payload) - offset,
                "server_requests_total{worker=\"%d\",request_type=\"ping\"} %llu\n"
                "server_requests_total{worker=\"%d\",request_type=\"burn\"} %llu\n"
                "server_requests_total{worker=\"%d\",request_type=\"stream\"} %llu\n"
                "server_requests_total{worker=\"%d\",request_type=\"unknown\"} %llu\n",
                workers[i].id, atomic_load(&workers[i].total_ping_requests),
                workers[i].id, atomic_load(&workers[i].total_burn_requests),
                workers[i].id, atomic_load(&workers[i].total_stream_requests),
                workers[i].id, atomic_load(&workers[i].total_unknown_requests));
        }

        char http_resp[16384];
        int len = snprintf(http_resp, sizeof(http_resp),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain; version=0.0.4\r\n"
            "Connection: close\r\n"
            "Content-Length: %d\r\n\r\n"
            "%s",
            offset, payload);

        write(client_fd, http_resp, len);
        close(client_fd);
    }
    return NULL;
}

void* worker_thread(void* arg) {
    worker_ctx_t *worker = (worker_ctx_t*)arg;
    struct epoll_event events[MAX_EVENTS];

    printf("Worker %d awaiting I/O events (epoll_fd=%d)\n", worker->id, worker->epoll_fd);

    while (1) {
        int nfds = epoll_wait(worker->epoll_fd, events, MAX_EVENTS, -1);

        for (int i = 0; i < nfds; i++) {
            int current_fd = events[i].data.fd;
            int is_alive = 1;

            if (events[i].events & EPOLLIN) {
                is_alive = handle_read(worker, current_fd);
            }
            else if (events[i].events & EPOLLOUT) {
                is_alive = handle_write(worker, current_fd);
            }

            if (!is_alive || (events[i].events & (EPOLLERR | EPOLLHUP))) {
                epoll_ctl(worker->epoll_fd, EPOLL_CTL_DEL, current_fd, NULL);
                close(current_fd);
                atomic_fetch_sub(&worker->active_connections, 1);
            }
        }
    }
    return NULL;
}

int main() {
    signal(SIGPIPE, SIG_IGN);

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    if (sched_getaffinity(0, sizeof(cpu_set_t), &cpuset) == -1) {
        perror("sched_getaffinity");
        return EXIT_FAILURE;
    }

    num_cores_global = CPU_COUNT(&cpuset);
    active_threads = num_cores_global + 1; // n workers + 1 acceptor

    int listener = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    make_socket_non_blocking(listener);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return EXIT_FAILURE;
    }

    if (listen(listener, MAX_FDS) == -1) {
        perror("listen");
        return EXIT_FAILURE;
    }

    printf("DUMB epoll server with %ld workers listening on 0.0.0.0:%d\n", num_cores_global, PORT);

    workers = calloc(num_cores_global, sizeof(worker_ctx_t));

    for (int i = 0; i < num_cores_global; i++) {
        workers[i].id = i;
        workers[i].epoll_fd = epoll_create1(0);
        pthread_create(&workers[i].thread_id, NULL, worker_thread, &workers[i]);
    }

    pthread_t reporter_tid;
    pthread_create(&reporter_tid, NULL, worker_metrics_server_thread, NULL);

    int acceptor_epoll = epoll_create1(0);
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = listener;
    epoll_ctl(acceptor_epoll, EPOLL_CTL_ADD, listener, &ev);

    int rr_counter = 0;
    struct epoll_event events[MAX_EVENTS];

    printf("Main thread accepting connections (epoll_fd=%d)\n", acceptor_epoll);

    while (1) {
        int nfds = epoll_wait(acceptor_epoll, events, MAX_EVENTS, -1);

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == listener) {
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

                    // Round-Robin
                    int target_worker = rr_counter % num_cores_global;
                    rr_counter++;

                    struct epoll_event client_ev;
                    client_ev.events = EPOLLIN;
                    client_ev.data.fd = client_fd;

                    epoll_ctl(workers[target_worker].epoll_fd, EPOLL_CTL_ADD, client_fd, &client_ev);

                    atomic_fetch_add(&workers[target_worker].active_connections, 1);
                }
            }
        }
    }

    return EXIT_SUCCESS;
}