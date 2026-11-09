/**
 * \brief Pipe wrapped in stream object example.
 *
 * Main process creates pipe and forks child process.
 * Both processes wrap raw pipe descriptor into a standard C stream object (FILE*).
 * Then formatted message is sent from child to the parent via pipe.
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
            FILE* stream = fdopen(fd[1], "w");
            setvbuf(stream, NULL, _IONBF, 0);
            fprintf(stream, "Nice, formatting possible again: %d!\n", 123);
             printf("Sleeping...\n");
             sleep(3);
            fclose(stream);
            break;
        }
        default: {
            // parent
            close(fd[1]); // close write-end
            FILE* stream = fdopen(fd[0], "r");
            int ret = 1;
            while (ret > 0) {
                char buf[16];
                ret = fscanf(stream, "%15s", buf);
                printf("scanf() returned %d: '%s'\n", ret, buf);
            }
            fclose(stream);
            while (wait(NULL) > 0);
            break;
        }
    }

    return 0;
}
