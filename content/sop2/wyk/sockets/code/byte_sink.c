#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

void wait_for_user(const char* message, const char* next_syscall) {
    printf("%s\n>>> [Press Enter to %s()] <<<", message, next_syscall);
    getchar();
}

int main() {
    /* 1. Create a TCP socket */
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    /* 2. Bind the socket to all interfaces on port 8080 */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind() failed");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    wait_for_user("Bound port 8080!", "listen");

    /* 3. Start listening for incoming connections */
    if (listen(listen_fd, 1) < 0) {
        perror("listen failed");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    wait_for_user("Listening on port 8080!", "accept");

    /* 4. Accept a connection */
    int client_fd = accept(listen_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept() failed");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    wait_for_user("Connection accepted!", "recv");

    /* 5. Read from the stream byte-by-byte */
    char buf;
    ssize_t n;

    while ((n = recv(client_fd, &buf, 1, 0)) > 0) {
        char msg[100];
        if (buf == '\n')
            snprintf(msg, sizeof(msg), "Received '\\n'");
        else
            snprintf(msg, sizeof(msg), "Received '%c'", buf);
        wait_for_user(msg, "recv");
    }

    if (n == 0) {
        printf("Client closed connection\n");
    } else if (n < 0) {
        perror("recv() failed");
    }

    /* 6. Clean up */
    close(client_fd);
    close(listen_fd);

    printf("Sockets closed\n");

    return 0;
}
