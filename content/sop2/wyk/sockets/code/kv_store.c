#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "kv_protocol.h"

#define MAX_ENTRIES 100

/* Simple in-memory database */
struct kv_entry {
    uint32_t key;
    char value[256];
    int in_use;
};
struct kv_entry db[MAX_ENTRIES];

/* Find an entry by key. Returns index or -1 if not found. */
int find_entry(uint32_t key) {
    for (int i = 0; i < MAX_ENTRIES; i++) {
        if (db[i].in_use && db[i].key == key) return i;
    }
    return -1;
}

int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket() failed");
        return EXIT_FAILURE;
    }

    int opt = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt() failed");
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8085);

    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind() failed");
        return EXIT_FAILURE;
    }

    if (listen(listen_fd, 5) < 0) {
        perror("listen() failed");
        return EXIT_FAILURE;
    }

    memset(db, 0, sizeof(db));
    printf("KV Store listening on port 8085 (Iterative Mode)\n");

    /* Outer Loop: Accept one client at a time */
    while (1) {
        printf("Waiting for client... ");
        fflush(stdout);
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            perror("accept() failed");
            continue;
        }

        char ip_text[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip_text, sizeof(ip_text));

        printf(" connected from %s:%d!\n", ip_text, ntohs(client_addr.sin_port));

        /* Inner Loop: Serve the connected client until they disconnect */
        while (1) {
            struct request_hdr req;

            // 1. Read the binary request header (Blocking!)
            ssize_t n = recv(client_fd, &req, sizeof(req), MSG_WAITALL);
            if (n <= 0) {
                if (n < 0) perror("recv() failed");
                break; // Client disconnected or error
            }

            // Convert network byte order to host byte order
            uint32_t key = ntohl(req.key);
            uint32_t val_len = ntohl(req.val_len);

            struct response_hdr res = { .status = STATUS_OK, .val_len = 0 };
            char buf[256] = {0};

            // 2. Process the request based on OP code
            if (req.op == OP_WRITE) {
                if (val_len >= sizeof(buf)) val_len = sizeof(buf) - 1;

                // Read the payload (value string)
                recv(client_fd, buf, val_len, MSG_WAITALL);

                int idx = find_entry(key);
                if (idx == -1) { // Find empty slot
                    for (int i = 0; i < MAX_ENTRIES; i++) {
                        if (!db[i].in_use) { idx = i; break; }
                    }
                }

                if (idx != -1) {
                    db[idx].key = key;
                    db[idx].in_use = 1;
                    strncpy(db[idx].value, buf, val_len);
                    db[idx].value[val_len] = '\0';
                    printf(" -> WRITE: Key %u = '%s'\n", key, db[idx].value);
                } else {
                    res.status = STATUS_ERROR; // Database full
                }

                send(client_fd, &res, sizeof(res), 0);

            } else if (req.op == OP_READ) {
                int idx = find_entry(key);
                if (idx != -1) {
                    res.val_len = htonl(strlen(db[idx].value));
                    send(client_fd, &res, sizeof(res), 0);
                    send(client_fd, db[idx].value, strlen(db[idx].value), 0);
                    printf(" -> READ: Key %u found.\n", key);
                } else {
                    res.status = STATUS_NOT_FOUND;
                    send(client_fd, &res, sizeof(res), 0);
                    printf(" -> READ: Key %u NOT FOUND.\n", key);
                }

            } else if (req.op == OP_DELETE) {
                int idx = find_entry(key);
                if (idx != -1) {
                    db[idx].in_use = 0;
                    printf(" -> DELETE: Key %u removed.\n", key);
                } else {
                    res.status = STATUS_NOT_FOUND;
                }
                send(client_fd, &res, sizeof(res), 0);
            } else if (req.op == OP_FIRE) {
                printf(" -> FIRE\n");
                printf("Busy for 3 seconds (1)... ");
                sleep(3);
                printf("done!\n");

                if (send(client_fd, &res, sizeof(res), 0) < 0) {
                    perror("send() failed");
                }

                printf("Busy for 3 seconds (2)... ");
                sleep(3);
                printf("done!\n");

                char text[] = "Fire!";
                res.status = STATUS_OK;
                res.val_len = htonl(strlen(text));

                // Próba 2: Śmiertelny Cios. Gniazdo ma już flagę błędu od RST. SIGPIPE!
                if (send(client_fd, text, strlen(text), 0) < 0) {
                    perror("send() failed"); // To się nigdy nie wypisze (bez SIG_IGN)
                }
            }
        }

        printf("Client disconnected. Closing socket.\n");
        close(client_fd);
    }

    close(listen_fd);
    return EXIT_SUCCESS;
}
