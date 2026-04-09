#include <arpa/inet.h>
#include <asm-generic/errno-base.h>
#include <endian.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "time_protocol.h"

uint64_t get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)(tv.tv_sec) * 1000 + (uint64_t)(tv.tv_usec) / 1000;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <PORT>\n", argv[0]);
        fprintf(stderr, "Example: %s 0.0.0.0 8080\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *ip_arg = argv[1];
    int port_arg = atoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_arg);

    // inet_pton converts text IP like "10.0.1.1" to binary network format
    if (inet_pton(AF_INET, ip_arg, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("[Server] Listening on %s:%d...\n", ip_arg, port_arg);

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    struct time_msg msg;

    while (1) {
        // 1. Receive Datagram
        ssize_t n = recvfrom(sockfd, &msg, sizeof(msg), 0,
                             (struct sockaddr *)&client_addr, &addr_len);
        if (n < 0) {
            perror("recvfrom failed");
            if (errno == EINTR) break;
            continue;
        }

        // 2. Deserialize (Network to Host Byte Order)
        uint8_t type = msg.msg_type;
        uint32_t seq = be32toh(msg.seq_id);
        uint64_t client_time = be64toh(msg.timestamp_ms);
        (void)client_time;

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);

        if (type == MSG_TYPE_REQ) {
            printf("[Server] Rx REQ | Seq: %u | From: %s:%d\n", seq, client_ip, ntohs(client_addr.sin_port));

            // 3. Prepare Response
            msg.msg_type = MSG_TYPE_RES;
            // seq_id remains the same, we just echo it back. It's already in Network Byte Order!

            uint64_t server_time = get_time_ms();
            msg.timestamp_ms = htobe64(server_time); // Convert new time to Network Order

            // 4. Send Response
            sendto(sockfd, &msg, sizeof(msg), 0,
                   (const struct sockaddr *)&client_addr, addr_len);
        }
    }

    close(sockfd);
    return 0;
}