#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <endian.h>
#include <errno.h>
#include "time_protocol.h"

uint64_t get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)(tv.tv_sec) * 1000 + (uint64_t)(tv.tv_usec) / 1000;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <Server IP> <Server PORT> [count]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    int count = 5;

    if (argc > 3) count = atoi(argv[3]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    struct time_msg msg;
    uint32_t current_seq = 1;

    for (int i = 0; i < count; i++) {
        // 1. Prepare Request
        msg.msg_type = MSG_TYPE_REQ;
        msg.seq_id = htobe32(current_seq);
        uint64_t start_time = get_time_ms();
        msg.timestamp_ms = htobe64(start_time);

        printf("[Client] Sending REQ | Seq: %u\n", current_seq);

        // 2. Send Datagram
        sendto(sockfd, &msg, sizeof(msg), 0,
               (const struct sockaddr *)&server_addr, sizeof(server_addr));

        // 3. Wait for Response
        struct sockaddr_in from_addr;
        socklen_t addr_len = sizeof(from_addr);

        ssize_t n = recvfrom(sockfd, &msg, sizeof(msg), 0,
                             (struct sockaddr *)&from_addr, &addr_len);

        if (n < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                printf("[Client] Timeout! Packet loss detected for Seq: %u\n", current_seq);
            } else {
                perror("recvfrom failed");
            }
        } else {
            // Deserialize Response
            uint32_t recv_seq = be32toh(msg.seq_id);
            uint64_t server_time = be64toh(msg.timestamp_ms);
            uint64_t end_time = get_time_ms();

            // Note: In UDP, packets can be reordered!
            printf("[Client] Rx RES | Seq: %u | Server Time: %llu | RTT: %llu ms\n",
                   recv_seq, (unsigned long long)server_time, (unsigned long long)(end_time - start_time));
        }

        current_seq++;
        sleep(1); // Wait a second before sending the next one
    }

    close(sockfd);
    return 0;
}
