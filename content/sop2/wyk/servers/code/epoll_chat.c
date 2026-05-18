#define _GNU_SOURCE // For accept4()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <signal.h>

#define PORT 8091
#define BUF_SIZE 1024
#define MAX_EVENTS 10

void add_client(int** pclients, int fd)
{
    int* clients = *pclients;
    int cap = clients[0];
    for (int i = 1; i < cap; i++)
    {
        if (clients[i] == -1)
        {
            clients[i] = fd;
            return;
        }
    }
    // Reallocate
    *pclients = realloc(clients, 2 * cap * sizeof(int));
    if (!*pclients) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    clients = *pclients;
    clients[0] = 2 * cap;
    clients[cap] = fd;
    for (int i = cap + 1; i < 2 * cap; i++) {
        clients[i] = -1;
    }
}

void remove_client(int* clients, int fd)
{
    for (int i = 1; i < clients[0]; i++)
    {
        if (clients[i] == fd)
        {
            clients[i] = -1;
            return;
        }
    }
}

int main() {
    signal(SIGPIPE, SIG_IGN);

    int listener = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (listener < 0) {
        perror("socket() failed");
        return EXIT_FAILURE;
    }

    int opt = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind() failed");
        return EXIT_FAILURE;
    }

    if (listen(listener, 10) < 0) {
        perror("listen() failed");
        return EXIT_FAILURE;
    }

    printf("Listening on 0.0.0.0:%d\n", PORT);

    // Create epoll instance
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1() failed");
        return EXIT_FAILURE;
    }

    struct epoll_event ev, events[MAX_EVENTS];

    // Register main server socket
    ev.events = EPOLLIN;
    ev.data.fd = listener;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listener, &ev) == -1) {
        perror("epoll_ctl: listener");
        return EXIT_FAILURE;
    }

    const int DEFAULT_CLIENTS_CAP = 2;
    int* clients = malloc( DEFAULT_CLIENTS_CAP * sizeof(int));
    if (!clients) {
        perror("malloc");
        return EXIT_FAILURE;
    }
    clients[0] = DEFAULT_CLIENTS_CAP; // Capacity in the first cell
    for (int i = 1; i < DEFAULT_CLIENTS_CAP; i++) clients[i] = -1;

    while (1) {
        printf("Calling epoll_wait()... ");
        fflush(stdout);

        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait() failed");
            break;
        }

        printf(" done! %d descriptors ready!\n", nfds);

        // Iterate over ready descriptors
        for (int n = 0; n < nfds; ++n) {
            int current_fd = events[n].data.fd;

            if (current_fd == listener) {
                printf("Calling accept()... ");
                fflush(stdout);

                struct sockaddr_in client_addr;
                socklen_t addrlen = sizeof(client_addr);
                int new_fd = accept4(listener, (struct sockaddr*)&client_addr, &addrlen, SOCK_NONBLOCK);

                if (new_fd == -1) {
                    perror("accept() failed");
                } else {
                    printf("done!: new client (fd=%d) connected from %s:%d\n",
                        new_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                    // Register new client
                    ev.events = EPOLLIN;
                    ev.data.fd = new_fd;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_fd, &ev) == -1) {
                        perror("epoll_ctl(EPOLL_CTL_ADD) failed");
                        close(new_fd);
                    } else {
                        add_client(&clients, new_fd);

                        char welcome[32];
                        snprintf(welcome, sizeof(welcome), "Welcome chat user no. %d!\n", new_fd);
                        send(new_fd, welcome, strlen(welcome), 0);
                    }
                }
            } else {
                printf("Calling recv(fd=%d)... ", current_fd);
                fflush(stdout);

                char buf[BUF_SIZE];
                int bytes_received = recv(current_fd, buf, BUF_SIZE - 1, 0);

                if (bytes_received <= 0) {
                    if (bytes_received == 0) {
                        printf("done! client disconnected!\n");
                    } else {
                        perror("recv() failed");
                    }

                    // Unregister client
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, current_fd, NULL);
                    close(current_fd);

                    remove_client(clients, current_fd);
                } else {
                    buf[bytes_received] = '\0';
                    printf("done! client received %d bytes!\n", bytes_received);

                    // Broadcast message to others
                    for (int j = 1; j < clients[0]; j++) {
                        if (clients[j] != -1 && clients[j] != current_fd) {
                            char out_buf[BUF_SIZE + 32];
                            snprintf(out_buf, sizeof(out_buf), "[User %d]: %s", current_fd, buf);
                            if (send(clients[j], out_buf, strlen(out_buf), 0) == -1) {
                                perror("send() failed");
                            }
                        }
                    }
                }
            }
        }
    }

    close(epoll_fd);
    close(listener);
    return 0;
}
