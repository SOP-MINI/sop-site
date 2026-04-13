#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "kv_protocol.h"

void print_help() {
    printf("Commands:\n");
    printf("  write <key> <value>\n");
    printf("  read <key>\n");
    printf("  delete <key>\n");
    printf("  quit\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0) {
        perror("inet_pton() failed");
        exit(EXIT_FAILURE);
    }

    printf("Connecting to KV store at %s:%d... ", argv[1], atoi(argv[2]));
    fflush(stdout);
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect() failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected!\n");
    print_help();

    char line[512];
    char val[256];
    uint32_t key;

    /* Interactive shell loop */
    while (1)
    {
        printf("\nkv> ");
        if (!fgets(line, sizeof(line), stdin)) break; // EOF (Ctrl+D)

        if (strncmp(line, "quit", 4) == 0) break;

        struct request_hdr req;
        memset(&req, 0, sizeof(req));
        int payload_len = 0;

        if (sscanf(line, "write %u %s", &key, val) == 2) {
            req.op = OP_WRITE;
            payload_len = strlen(val);
        } else if (sscanf(line, "read %u", &key) == 1) {
            req.op = OP_READ;
        } else if (sscanf(line, "delete %u", &key) == 1) {
            req.op = OP_DELETE;
        } else if (sscanf(line, "fire") == 0) {
            req.op = OP_FIRE;
        } else {
            printf("Invalid command.\n");
            continue;
        }

        // Convert host byte order to network byte order
        req.key = htonl(key);
        req.val_len = htonl(payload_len);

        // Send binary header
        if (send(sockfd, &req, sizeof(req), 0) < 0) {
            perror("send() failed");
            break;
        }

        if (req.op == OP_FIRE) {
            exit(EXIT_SUCCESS);
        }

        // Send payload if WRITE
        if (req.op == OP_WRITE) {
            send(sockfd, val, payload_len, 0);
        }

        // Read binary response header
        struct response_hdr res;
        if (recv(sockfd, &res, sizeof(res), MSG_WAITALL) <= 0) {
            printf("Server closed connection.\n");
            break;
        }

        if (res.status == STATUS_OK) {
            if (req.op == OP_READ) {
                uint32_t r_len = ntohl(res.val_len);
                char res_val[256] = {0};
                recv(sockfd, res_val, r_len, MSG_WAITALL);
                printf("SUCCESS: Value = '%s'\n", res_val);
            } else {
                printf("SUCCESS.\n");
            }
        } else if (res.status == STATUS_NOT_FOUND) {
            printf("ERROR: Key not found.\n");
        } else {
            printf("ERROR: Internal server error.\n");
        }
    }

    close(sockfd);
    return EXIT_SUCCESS;
}