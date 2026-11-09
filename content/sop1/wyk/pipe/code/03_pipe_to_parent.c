/**
 * \brief Pipe from child to parent process example.
 *
 * Main process creates pipe and forks child process.
 * Then message is sent from child to the parent via pipe.
 */

#include <unistd.h>
#include <stdio.h>
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
            close(fd[0]); // close read-end
            ssize_t ret = write(fd[1], "text", 5);
            if (ret < 0) {
                perror("write(): ");
                return 1;
            }
            printf("[%d] write() returned %ld bytes\n", getpid(), ret);
            close(fd[1]);
            break;
        }
        default: {
            // parent
            close(fd[1]); // close write-end

            char buf[16];

            ssize_t ret = read(fd[0], buf, sizeof(buf));
            if (ret < 0) {
                perror("read(): ");
                return 1;
            }

            printf("[%d] read() returned %ld bytes: '%s'\n", getpid(), ret, buf);

            close(fd[0]);
            while (wait(NULL) > 0);
            break;
        }
    }

    return 0;
}
