/**
 * \brief Closed pipe detection by the reading site example.
 *
 * Main process creates pipe and forks child process.
 * Multiple messages are sent from parent to the child via pipe.
 * Child process detects end of transmission by checking that read()
 * returned 0 after parent had closed it's write end.
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main() {

    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe(): ");
        return 1;
    }

    // fd[0] represents read-end of the pipe
    // fd[1] represents write-end of the pipe

    pid_t pid = fork();
    switch (pid) {
        case -1:
            perror("fork(): ");
            return 1;
        case 0: {
            // child
            close(fd[1]); // close write-end

            while (1) {
                char buf[16];
                ssize_t ret = read(fd[0], buf, sizeof(buf));
                if (ret < 0) {
                    perror("read(): ");
                    return 1;
                }
                printf("[%d] read() returned %ld bytes\n", getpid(), ret);
                // if (ret == 0) {
                //     break;
                // }
            }

            close(fd[0]);
            break;
        }
        default: {
            // parent
            close(fd[0]); // close read-end

            for (int i = 0; i < 5; ++i) {
                ssize_t ret = write(fd[1], "text", 5);
                if (ret < 0) {
                    perror("write(): ");
                    return 1;
                }
                printf("[%d] write() returned %ld bytes\n", getpid(), ret);
                sleep(1);
            }

            printf("[%d] closing pipe\n", getpid());
            close(fd[1]);
            while (wait(NULL) > 0);
            break;
        }
    }

    return 0;
}
