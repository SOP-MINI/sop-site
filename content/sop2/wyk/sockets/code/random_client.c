#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <endian.h>
#include <stdint.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Usage: %s <IP> <LEN> [DELAY_MS]\n", argv[0]);
        fprintf(stderr, "Tip: Redirect stdout to /dev/null to avoid corrupting your terminal!\n");
        return EXIT_FAILURE;
    }

    const char *server_ip = argv[1];
    uint64_t req_len = strtoull(argv[2], NULL, 10);
    uint64_t req_delay = 0;
    if (argc == 4)
    {
        req_delay = strtoull(argv[3], NULL, 10);
    }

    uint64_t req = (req_len << 16) | req_delay;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket() failed");
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8089);

    if (inet_pton(AF_INET, server_ip, &addr.sin_addr) <= 0) {
        perror("inet_pton() failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    fprintf(stderr, "Connecting to %s:8089... ", server_ip);
    fflush(stderr);
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect() failed");
        close(sockfd);
        return EXIT_FAILURE;
    }
    fprintf(stderr, "Connected. Requesting %lu bytes with %lu ms delay...\n", req_len, req_delay);

    /* Send request */
    uint64_t net_req = htobe64(req);
    if (send(sockfd, &net_req, sizeof(net_req), 0) < 0) {
        perror("send() failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    char buf[BUF_SIZE];
    uint64_t total_received = 0;
    ssize_t n;

    /* Receive and print response */
    while (total_received < req_len) {
        n = recv(sockfd, buf, BUF_SIZE, 0);

        if (n < 0) {
            perror("recv() failed");
            break;
        }

        total_received += n;
        fprintf(stderr, "Received %ld/%lu random bytes\n", total_received, req_len);

        if (n == 0) {
            fprintf(stderr, "Server closed connection prematurely.\n");
            break;
        }

        // Piszemy odebrane bajty na standardowe wyjście (STDOUT_FILENO)
        if (write(STDOUT_FILENO, buf, n) < 0) {
            perror("write() failed");
            break;
        }
    }

    fprintf(stderr, "\nFinished. Received %lu bytes total.\n", total_received);
    close(sockfd);

    return EXIT_SUCCESS;
}