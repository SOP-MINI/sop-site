#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <IP> <PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    /* 1. Create socket */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip, &addr.sin_addr) <= 0)
    {
        perror("inet_pton() failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    /* 1. Connect */
    printf("[Connecting to %s:%d... ", server_ip, server_port);
    fflush(stdout);
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect() failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected!\n");

    char text[128];

    /* 2. Ręczne wysyłanie pakiet po pakiecie (bajt po bajcie) */
    while (1)
    {
        printf("> ");
        fflush(stdout);

        if (fgets(text, sizeof(text), stdin) == NULL)
            break;

        // Wysyłamy całą linię
        if (send(sockfd, text, strlen(text), 0) < 0)
        {
            perror("send() failed");
            break;
        }
    }

    /* 3. Zamykanie */
    printf("Closing socket... ");
    fflush(stdout);
    close(sockfd);
    printf("Closed!\n");

    return EXIT_SUCCESS;
}