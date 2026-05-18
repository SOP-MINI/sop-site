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
    client_state_t state;
    char read_buf[1024];
    size_t read_len;
    long bytes_to_send;
    long bytes_sent;
} client_ctx_t;

client_ctx_t clients[MAX_FDS];

typedef struct worker_ctx {
    int id;
    int epoll_fd;
    pthread_t thread_id;
} worker_ctx_t;

worker_ctx_t* workers;
long num_cores_global = 0;

typedef struct handler handler_t;
typedef void (*event_cb)(worker_ctx_t *worker, handler_t *item);

struct handler {
    int fd;
    // Callbacks (inversion of control)
    event_cb on_read;
    event_cb on_write;
    void *app_data; // Transparent application context
};

handler_t reactor_registry[MAX_FDS];

static int make_socket_non_blocking(int sfd) {
    int flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) return -1;
    flags |= O_NONBLOCK;
    if (fcntl(sfd, F_SETFL, flags) == -1) return -1;
    return 0;
}

void init_client_ctx(int fd) {
    clients[fd].state = STATE_READ_COMMAND;
    clients[fd].read_len = 0;
    clients[fd].bytes_to_send = 0;
    clients[fd].bytes_sent = 0;
}

void reactor_modify(int epollfd, handler_t *item, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = item;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, item->fd, &ev);
}

void reactor_remove(worker_ctx_t *worker, handler_t *item) {
    epoll_ctl(worker->epoll_fd, EPOLL_CTL_DEL, item->fd, NULL);
    close(item->fd);
    item->on_read = NULL;
    item->on_write = NULL;
    item->app_data = NULL;
}

void client_read_handler(worker_ctx_t *worker, handler_t *item) {
    client_ctx_t *ctx = (client_ctx_t *)item->app_data;
    ssize_t count = read(item->fd, ctx->read_buf + ctx->read_len, sizeof(ctx->read_buf) - 1 - ctx->read_len);

    if (count == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) return;
        reactor_remove(worker, item);
        return;
    }

    if (count == 0) {
        reactor_remove(worker, item);
        return;
    }

    ctx->read_len += count;
    ctx->read_buf[ctx->read_len] = '\0';

    char *newline = strchr(ctx->read_buf, '\n');
    if (newline) {
        *newline = '\0';

        if (strncmp(ctx->read_buf, "PING", 4) == 0) {
            ctx->state = STATE_WRITE_PONG;
            ctx->bytes_sent = 0;
            ctx->bytes_to_send = 5;
        }
        else if (strncmp(ctx->read_buf, "BURN", 4) == 0) {
            long long depth = 0;
            sscanf(ctx->read_buf, "BURN %lld", &depth);
            long long sum = recursive_burn(depth);
            ctx->state = STATE_WRITE_BURN;
            ctx->bytes_sent = 0;
            ctx->bytes_to_send = snprintf(ctx->read_buf, sizeof(ctx->read_buf), "RESULT %lld\n", sum);
        }
        else if (strncmp(ctx->read_buf, "STREAM", 6) == 0) {
            long bytes = 65536;
            sscanf(ctx->read_buf, "STREAM %ld", &bytes);
            ctx->state = STATE_WRITE_STREAM;
            ctx->bytes_sent = 0;
            ctx->bytes_to_send = bytes;
        }
        else {
            ctx->read_len = 0;
            return;
        }
        reactor_modify(worker->epoll_fd, item, EPOLLOUT);
    } else if (ctx->read_len == sizeof(ctx->read_buf) - 1) {
        ctx->read_len = 0;
    }
}

void client_write_handler(worker_ctx_t *worker, handler_t *item) {
    client_ctx_t *ctx = (client_ctx_t *)item->app_data;

    if (ctx->state == STATE_WRITE_PONG) {
        char *msg = "PONG\n";
        long remaining = ctx->bytes_to_send - ctx->bytes_sent;
        ssize_t written = write(item->fd, msg + ctx->bytes_sent, remaining);
        if (written < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) return;
            reactor_remove(worker, item);
            return;
        }
        ctx->bytes_sent += written;

        if (ctx->bytes_sent == ctx->bytes_to_send) {
            ctx->state = STATE_READ_COMMAND;
            ctx->read_len = 0;
            reactor_modify(worker->epoll_fd, item, EPOLLIN);
        }
    }
    else if (ctx->state == STATE_WRITE_BURN) {
        long remaining = ctx->bytes_to_send - ctx->bytes_sent;
        ssize_t written = write(item->fd, ctx->read_buf + ctx->bytes_sent, remaining);
        if (written < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) return;
            reactor_remove(worker, item);
            return;
        }
        ctx->bytes_sent += written;

        if (ctx->bytes_sent == ctx->bytes_to_send) {
            ctx->state = STATE_READ_COMMAND;
            ctx->read_len = 0;
            reactor_modify(worker->epoll_fd, item, EPOLLIN);
        }
    }
    else if (ctx->state == STATE_WRITE_STREAM) {
        char chunk[16384];
        memset(chunk, 'A', sizeof(chunk));
        long remaining = ctx->bytes_to_send - ctx->bytes_sent;
        size_t to_write = (remaining < sizeof(chunk)) ? remaining : sizeof(chunk);

        ssize_t written = write(item->fd, chunk, to_write);
        if (written < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) return;
            reactor_remove(worker, item);
            return;
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

        ssize_t written = write(item->fd, msg + ctx->bytes_sent, remaining);
        if (written < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) return;
            reactor_remove(worker, item);
            return;
        }
        ctx->bytes_sent += written;

        if (ctx->bytes_sent == ctx->bytes_to_send) {
            ctx->state = STATE_READ_COMMAND;
            ctx->read_len = 0;
            reactor_modify(worker->epoll_fd, item, EPOLLIN);
        }
    }
}

void accept_handler(worker_ctx_t *ignored, handler_t *listener_item) {
    static int rr_counter = 0;

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int client_fd = accept(listener_item->fd, (struct sockaddr*)&client_addr, &addrlen);

        if (client_fd == -1) break;

        if (client_fd >= MAX_FDS) {
            close(client_fd);
            continue;
        }

        make_socket_non_blocking(client_fd);
        init_client_ctx(client_fd);

        int target_worker = rr_counter % num_cores_global;
        rr_counter++;

        handler_t *client_item = &reactor_registry[client_fd];
        client_item->fd = client_fd;
        client_item->on_read = client_read_handler;
        client_item->on_write = client_write_handler;
        client_item->app_data = &clients[client_fd];

        struct epoll_event client_ev;
        client_ev.events = EPOLLIN;
        client_ev.data.ptr = client_item;

        epoll_ctl(workers[target_worker].epoll_fd, EPOLL_CTL_ADD, client_fd, &client_ev);
    }
}

// Universal reactor event loop (aka. 'dispatcher')
void reactor_run(int epoll_fd, worker_ctx_t *worker_context) {
    struct epoll_event events[MAX_EVENTS];

    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for (int i = 0; i < nfds; i++) {
            // Obtain handler pointer (may point to any handler type)
            handler_t *item = (handler_t *)events[i].data.ptr;

            if (events[i].events & (EPOLLERR | EPOLLHUP)) {
                reactor_remove(worker_context, item);
                continue;
            }

            // Polymorphic callback invocation
            if (events[i].events & EPOLLIN && item->on_read) {
                item->on_read(worker_context, item);
            }
            else if (events[i].events & EPOLLOUT && item->on_write) {
                item->on_write(worker_context, item);
            }
        }
    }
}

void* worker_thread(void* arg) {
    worker_ctx_t *worker = (worker_ctx_t*)arg;
    printf("Worker %d starting dispatches events (epoll_fd=%d)\n", worker->id, worker->epoll_fd);
    reactor_run(worker->epoll_fd, worker);
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
    active_threads = num_cores_global + 1;

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

    printf("DUMB epoll reactor server with %ld workers listening on 0.0.0.0:%d\n", num_cores_global, PORT);

    workers = calloc(num_cores_global, sizeof(worker_ctx_t));

    for (int i = 0; i < num_cores_global; i++) {
        workers[i].id = i;
        workers[i].epoll_fd = epoll_create1(0);
        pthread_create(&workers[i].thread_id, NULL, worker_thread, &workers[i]);
    }

    int acceptor_epoll = epoll_create1(0);

    handler_t *listener_item = &reactor_registry[listener];
    listener_item->fd = listener;
    listener_item->on_read = accept_handler;
    listener_item->on_write = NULL;
    listener_item->app_data = NULL;

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = listener_item;
    epoll_ctl(acceptor_epoll, EPOLL_CTL_ADD, listener, &ev);

    printf("Main thread dispatches events (epoll_fd=%d)\n", acceptor_epoll);

    reactor_run(acceptor_epoll, NULL);

    return EXIT_SUCCESS;
}
