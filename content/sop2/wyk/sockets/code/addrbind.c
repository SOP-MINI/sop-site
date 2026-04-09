#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    // We require IP and Port. Hint: passing port 0 is the real magic here!
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP_ADDRESS> <PORT>\n", argv[0]);
        fprintf(stderr, "Hint: Use port 0 to let the OS choose a random free port!\n");
        return EXIT_FAILURE;
    }

    const char *ip_arg = argv[1];
    int port_arg = atoi(argv[2]);

    // 1. Create Socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket() failed");
        return EXIT_FAILURE;
    }

    // 2. Prepare the requested address structure
    struct sockaddr_in req_addr;
    memset(&req_addr, 0, sizeof(req_addr));
    req_addr.sin_family = AF_INET;
    req_addr.sin_port = htons(port_arg);

    if (inet_pton(AF_INET, ip_arg, &req_addr.sin_addr) <= 0) {
        perror("inet_pton() failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    // 3. Bind the socket
    if (bind(sockfd, (struct sockaddr *)&req_addr, sizeof(req_addr)) < 0) {
        perror("bind() failed");
        close(sockfd);
        return EXIT_FAILURE;
    }
    printf("Successfully called bind() requesting %s:%d\n", ip_arg, port_arg);

    // 4. Ask the kernel what address/port is ACTUALLY bound
    struct sockaddr_in bound_addr;
    socklen_t len = sizeof(bound_addr); // Crucial: value-result argument!

    if (getsockname(sockfd, (struct sockaddr *)&bound_addr, &len) == -1) {
        perror("getsockname() failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    // 5. Decode and print the result
    char result_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &bound_addr.sin_addr, result_ip, INET_ADDRSTRLEN);

    printf("Actual bound IP:   %s\n", result_ip);
    printf("Actual bound Port: %d\n", ntohs(bound_addr.sin_port));

    printf("Waiting for key press...\n");
    getchar();
    printf("Releasing the address...\n");

    close(sockfd);
    printf("Done!\n");
    return EXIT_SUCCESS;
}