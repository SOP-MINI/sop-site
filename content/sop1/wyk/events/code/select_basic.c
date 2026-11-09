#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/select.h>

int main() {
    int stdin_fd = STDIN_FILENO;
    fcntl(stdin_fd, F_SETFL, O_NONBLOCK);

    fd_set read_fds;
    char buffer[1024];

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(stdin_fd, &read_fds);

        printf("calling select()\n-> ");
        fflush(stdout);
        int activity = select(stdin_fd + 1, &read_fds, NULL, NULL, NULL);
        printf("returned %d\n", activity);
        if (activity < 0) {
            perror("select");
            break;
        }

        if (FD_ISSET(stdin_fd, &read_fds)) {
            ssize_t bytes_read = read(stdin_fd, buffer, sizeof(buffer) - 1);

            if (bytes_read == 0) {
                printf("read EOF\n");
                break;
            }
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                buffer[strcspn(buffer, "\n")] = 0;
                printf("read: '%s'\n", buffer);
            }
        }
    }

    printf("event loop finished\n");
    return 0;
}