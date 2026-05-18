#include "dumb_common.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

atomic_ullong active_threads = 0;
atomic_ullong active_connections = 0;
atomic_ullong total_ping_requests = 0;
atomic_ullong total_wait_requests = 0;
atomic_ullong total_burn_requests = 0;
atomic_ullong total_stream_requests = 0;
atomic_ullong total_unknown_requests = 0;

void *metrics_server_thread(void *arg) {
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

    char http_resp[2048];
    while(1) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) continue;

        // Read and ignore HTTP request content
        char dummy[1024];
        read(client_fd, dummy, sizeof(dummy));

        // Format prometheus compatible response
        int len = snprintf(http_resp, sizeof(http_resp),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain; version=0.0.4\r\n"
            "Connection: close\r\n\r\n"
            "# HELP server_active_threads Client threads\n"
            "# TYPE server_active_threads gauge\n"
            "server_active_threads %llu\n"
            "# HELP server_active_connections Active inbound connections\n"
            "# TYPE server_active_connections gauge\n"
            "server_active_connections %llu\n"
            "# HELP server_requests_total Received requests\n"
            "# TYPE server_requests_total counter\n"
            "server_requests_total{request_type=\"ping\"} %llu\n"
            "server_requests_total{request_type=\"wait\"} %llu\n"
            "server_requests_total{request_type=\"burn\"} %llu\n"
            "server_requests_total{request_type=\"stream\"} %llu\n"
            "server_requests_total{request_type=\"unknown\"} %llu\n",
            atomic_load(&active_threads),
            atomic_load(&active_connections),
            atomic_load(&total_ping_requests),
            atomic_load(&total_wait_requests),
            atomic_load(&total_burn_requests),
            atomic_load(&total_stream_requests),
            atomic_load(&total_unknown_requests)
        );

        write(client_fd, http_resp, len);
        close(client_fd);
    }
    return NULL;
}

#define BUF_SIZE 1024
#define STREAM_CHUNK_SIZE 4096

// BURN command helper
long long recursive_burn(long long depth) {
    if (depth <= 0) {
        return 0;
    }

    volatile char stack_eater[256] __attribute__((unused));
    stack_eater[0] = (char)(depth % 256);

    return depth + recursive_burn(depth - 1);
}

// Blocking I/O client handler
void handle_client(int client_fd) {
    char buf[BUF_SIZE];
    int buf_len = 0;
    ssize_t bytes_read;

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char client_addr_txt[INET_ADDRSTRLEN];
    memset(&client_addr, 0, sizeof(client_addr));
    getpeername(client_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_txt, INET_ADDRSTRLEN);

    while (1) {
        bytes_read = recv(client_fd, buf + buf_len, BUF_SIZE - buf_len - 1, 0);

        if (bytes_read <= 0) {
            break;
        }

        buf_len += bytes_read;
        buf[buf_len] = '\0';

        char *newline;
        while ((newline = strchr(buf, '\n')) != NULL) {
            *newline = '\0';
            size_t cmd_len = newline - buf;

            if (strncmp(buf, "PING", 4) == 0)
            {
                atomic_fetch_add(&total_ping_requests, 1);
                send(client_fd, "PONG\n", 5, 0);
            }
            else if (strncmp(buf, "WAIT ", 5) == 0)
            {
                atomic_fetch_add(&total_wait_requests, 1);
                int ms = atoi(buf + 5);
                usleep(ms * 1000);
                send(client_fd, "DONE\n", 5, 0);
            }
            else if (strncmp(buf, "BURN ", 5) == 0)
            {
                atomic_fetch_add(&total_burn_requests, 1);

                long long depth = atoll(buf + 5);

                long long sum = recursive_burn(depth);

                char res[64];
                int len = snprintf(res, sizeof(res), "RESULT %lld\n", sum);
                send(client_fd, res, len, 0);
            }
            else if (strncmp(buf, "STREAM ", 7) == 0)
            {
                atomic_fetch_add(&total_stream_requests, 1);

                long long bytes_to_send = atoll(buf + 7);
                char chunk[STREAM_CHUNK_SIZE];
                memset(chunk, 'A', STREAM_CHUNK_SIZE);

                while (bytes_to_send > 0)
                {
                    size_t to_send = (bytes_to_send > STREAM_CHUNK_SIZE) ? STREAM_CHUNK_SIZE : bytes_to_send;
                    ssize_t sent = send(client_fd, chunk, to_send, 0);

                    if (sent < 0)
                    {
                        break;
                    }
                    bytes_to_send -= sent;
                }

                if (bytes_to_send <= 0)
                {
                    send(client_fd, "DONE\n", 5, 0);
                }
            }
            else
            {
                atomic_fetch_add(&total_unknown_requests, 1);
                send(client_fd, "UNKNOWN_COMMAND\n", 16, 0);
            }

            int remaining = buf_len - cmd_len - 1;
            if (remaining > 0) {
                memmove(buf, newline + 1, remaining);
            }
            buf_len = remaining;
            buf[buf_len] = '\0';
        }

        if (buf_len >= BUF_SIZE - 1) {
            buf_len = 0;
        }
    }

    close(client_fd);
}
