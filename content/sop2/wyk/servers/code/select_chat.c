#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>

#define PORT 8090
#define BUF_SIZE 1024

int main() {
    signal(SIGPIPE, SIG_IGN);

    int listener = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (listener < 0) {
        perror("socket() failed");
        return EXIT_FAILURE;
    }

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

    fd_set master_set;
    fd_set read_fds;

    FD_ZERO(&master_set);
    FD_ZERO(&read_fds);

    FD_SET(listener, &master_set);
    int fd_max = listener;
    while (1) {
        read_fds = master_set;

        printf("Calling select()... ");
        fflush(stdout);

        int ret;
        if ((ret = select(fd_max + 1, &read_fds, NULL, NULL, NULL)) == -1) {
            perror("select() failed");
            break;
        }

        printf(" done! %d descriptors ready!\n", ret);

        for (int i = 0; i <= fd_max; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == listener) {
                    printf("Calling accept()... ");
                    fflush(stdout);

                    struct sockaddr_in client_addr;
                    socklen_t addrlen = sizeof(client_addr);
                    int new_fd = accept(listener, (struct sockaddr*)&client_addr, &addrlen);

                    if (new_fd == -1) {
                        perror("accept() failed");
                    } else {
                        FD_SET(new_fd, &master_set);
                        if (new_fd > fd_max) {
                            fd_max = new_fd;
                        }

                        printf("done!: new client (fd=%d) connected from %s:%d\n",
                               new_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                        char welcome[32];
                        snprintf(welcome, sizeof(welcome), "Welcome chat user no. %d!\n", new_fd);
                        send(new_fd, welcome, strlen(welcome), 0);
                    }
                } else {
                    printf("Calling recv(fd=%d)... ", i);
                    fflush(stdout);

                    char buf[BUF_SIZE];
                    int bytes_received = recv(i, buf, BUF_SIZE - 1, 0);


                    if (bytes_received <= 0) {
                        if (bytes_received == 0) {
                            printf("done! client disconnected!\n");
                        } else {
                            perror("recv() failed");
                        }
                        close(i);
                        FD_CLR(i, &master_set);
                    } else {
                        buf[bytes_received] = '\0';
                        printf("done! client received %d bytes!\n", bytes_received);

                        // Broadcast message to others
                        for (int j = 0; j <= fd_max; j++) {
                            if (FD_ISSET(j, &master_set)) {
                                if (j != listener && j != i) {
                                    char out_buf[BUF_SIZE + 32];
                                    snprintf(out_buf, sizeof(out_buf), "[User %d]: %s", i, buf);
                                    if (send(j, out_buf, strlen(out_buf), 0) == -1) {
                                        perror("send() failed");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}