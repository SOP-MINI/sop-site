/**
 * \brief Sticking write examples.
 *
 * Parent sends 10 x 10B messages to the child via pipe.
 * Child reads in most likely in a single read() and is unable to distinguish messages.
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>

int main() {

    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe(): ");
        return 1;
    }

    // fd[0] represents read-end of the pipe
    // fd[1] represents write-end of the pipe

    printf("PIPE_BUF = %d\n", PIPE_BUF);

    char buf[PIPE_BUF];

    pid_t pid = fork();
    switch (pid) {
        case -1:
            perror("fork(): ");
            return 1;
        case 0: {
            // child
            close(fd[1]); // close write-end

            while (1) {
                ssize_t ret = read(fd[0], buf, sizeof(buf));
                if (ret < 0) {
                    perror("read(): ");
                    return 1;
                } else if (ret == 0) {
                    break;
                }
                printf("[%d] read() returned %ld bytes\n", getpid(), ret);
            }

            close(fd[0]);
            break;
        }
        default: {
            // parent
            close(fd[0]); // close read-end

            memset(buf, 0, sizeof(buf));

            for (int i = 0; i < 10; ++i) {
                ssize_t ret = write(fd[1], buf, 10);
                if (ret < 0) {
                    perror("write(): ");
                    return 1;
                }
                printf("[%d] write() returned %ld bytes\n", getpid(), ret);
            }

            close(fd[1]);
            while (wait(NULL) > 0);
            break;
        }
    }

    return 0;
}
