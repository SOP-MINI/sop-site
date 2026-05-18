#include "dumb_common.h"

#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    signal(SIGPIPE, SIG_IGN);

    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return EXIT_FAILURE;
    }

    if (listen(listener, 10) < 0) {
        perror("listen");
        return EXIT_FAILURE;
    }

    printf("DUMB Iterative Server listening on 0.0.0.0:%d\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);

        int client_fd = accept(listener, (struct sockaddr*)&client_addr, &addrlen);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, INET_ADDRSTRLEN);
        printf("Connected: %s:%d (fd=%d)\n", ip, ntohs(client_addr.sin_port), client_fd);

        handle_client(client_fd);

        printf("Disconnected: %s:%d (fd=%d)\n", ip, ntohs(client_addr.sin_port), client_fd);
    }

    close(listener);
    return EXIT_SUCCESS;
}