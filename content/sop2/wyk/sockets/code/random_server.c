#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <endian.h>

int main() {
    int rand_fd = open("/dev/urandom", O_RDONLY);
    if (rand_fd < 0)
    {
        perror("open() failed");
        return EXIT_FAILURE;
    }

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
    addr.sin_port = htons(8089);

    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind() failed");
        return EXIT_FAILURE;
    }

    if (listen(listen_fd, 3) < 0) {
        perror("listen() failed");
        return EXIT_FAILURE;
    }

    printf("Random server listening on port 8089 (Iterative Mode)\n");

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

        uint64_t req;

        ssize_t n = recv(client_fd, &req, sizeof(req), MSG_WAITALL);
        if (n <= 0) {
            if (n < 0) perror("recv() failed");
            break; // Client disconnected or error
        }

        req = be64toh(req);
        uint64_t len = (req >> 16);
        uint64_t delay = (req & 0xFFFF);

        printf("Generating %lu random bytes with %lu ms delay\n", len, delay);

        uint64_t total_len = len;

        const size_t BS = 1024;
        char buf[BS];
        while (len > 0)
        {
            size_t bs = len > BS ? BS : len;
            n = read(rand_fd, buf, bs);
            if (n < 0)
            {
                perror("read() failed");
                break;
            }

            if (delay > 0) usleep(delay * 1000);

            n = send(client_fd, buf, n, 0);
            if (n < 0)
            {
                perror("send() failed");
                break;
            }

            len -= n;
            fprintf(stderr, "Sent %ld/%lu random bytes\n", total_len - len, total_len);
        }

        printf("Done. Closing socket.\n");
        close(client_fd);
    }

    close(listen_fd);
    return EXIT_SUCCESS;
}
