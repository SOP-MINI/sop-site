#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 8

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5678);

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        return EXIT_FAILURE;
    }

    printf("[Server] Listening on UDP port 5678...\n");

    char buffer[BUF_SIZE];
    socklen_t len = sizeof(client_addr);

    while (1) {
        // Czyścimy bufor, żeby nie było w nim śmieci z poprzednich odczytów
        memset(buffer, 0, BUF_SIZE);

        // KRYTYCZNY MOMENT: Odbieramy MAKSYMALNIE 16 bajtów
        ssize_t n = recvfrom(sockfd, buffer, BUF_SIZE, 0,
                             (struct sockaddr *)&client_addr, &len);

        if (n > 0) {
            // Wypisujemy tylko tyle, ile faktycznie przeczytaliśmy (żeby uniknąć krzaków)
            printf("[Server] Rx: %zd bytes | Text: '%.*s'\n", n, (int)n, buffer);

            // Odsyłamy to, co udało się przeczytać z powrotem do klienta
            sendto(sockfd, buffer, n, 0, (const struct sockaddr *)&client_addr, len);
        }
    }

    close(sockfd);
    return EXIT_SUCCESS;
}
