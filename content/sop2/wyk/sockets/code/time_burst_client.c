#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <endian.h>
#include <errno.h>
#include "time_protocol.h" // Upewnij się, że nazwa nagłówka zgadza się z Twoją!

uint64_t get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)(tv.tv_sec) * 1000 + (uint64_t)(tv.tv_usec) / 1000;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <Server IP> <Server PORT> [count] [delay_usec]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    int count = 10;
    int delay_usec = 10000;

    if (argc > 3) count = atoi(argv[3]);
    if (argc > 4) delay_usec = atoi(argv[4]);

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

    printf("[Client] --- STARTING BURST OF %d PACKETS ---\n", count);

    for (int i = 0; i < count; i++) {
        msg.msg_type = MSG_TYPE_REQ;
        msg.seq_id = htobe32(current_seq);
        msg.timestamp_ms = htobe64(get_time_ms());

        ssize_t n = sendto(sockfd, &msg, sizeof(msg), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
        if (n < 0) {
            perror("sendto() failed");
            break;
        }

        printf("[Client] Tx -> Seq: %u\n", current_seq);
        current_seq++;

        usleep(delay_usec);
    }

    printf("\n[Client] --- BURST SENT. LISTENING FOR RESPONSES ---\n");

    int received_count = 0;

    // Próbujemy odebrać dokładnie tyle pakietów, ile wysłaliśmy
    for (int i = 0; i < count; i++) {
        struct sockaddr_in from_addr;
        socklen_t addr_len = sizeof(from_addr);

        ssize_t n = recvfrom(sockfd, &msg, sizeof(msg), 0,
                             (struct sockaddr *)&from_addr, &addr_len);

        if (n < 0) {
            perror("recvfrom() failed");
            break;
        }

        uint32_t recv_seq = be32toh(msg.seq_id);
        uint64_t server_time = be64toh(msg.timestamp_ms);

        printf("[Client] Rx <- Seq: %u | Server Time: %llu\n", recv_seq, (unsigned long long)server_time);
        received_count++;
    }

    printf("\n[Client] Summary: Sent %d, Received %d. Lost %d (%.1f%%)\n",
           count, received_count, count - received_count,
           (float)(count - received_count) / count * 100.0);

    close(sockfd);
    return 0;
}