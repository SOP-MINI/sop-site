/**
 * \brief Deadlock on opening 2 FIFOs example.
 *
 * Two processes try to open 2 FIFOs in blocking mode.
 * Depending on the order this may result in a deadlock.
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#define FIFO1_NAME "my1.fifo"
#define FIFO2_NAME "my2.fifo"

int main() {

    unlink(FIFO1_NAME);
    unlink(FIFO2_NAME);

    if (mkfifo(FIFO1_NAME, 0666) < 0) {
        perror("mkfifo(): ");
        return 1;
    }

    if (mkfifo(FIFO2_NAME, 0666) < 0) {
        perror("mkfifo(): ");
        return 1;
    }

    int fd1, fd2;

    pid_t pid = fork();
    switch (pid) {
        case -1:
            perror("fork(): ");
            return 1;
        case 0: {
            // child
            printf("Trying to open FIFO '%s' for reading\n", FIFO1_NAME);
            if ((fd1 = open(FIFO1_NAME, O_RDONLY)) < 0) {
                perror("open(): ");
                return 1;
            }
            printf("Trying to open FIFO '%s' for reading\n", FIFO2_NAME);
            if ((fd2 = open(FIFO2_NAME, O_RDONLY)) < 0) {
                perror("open(): ");
                return 1;
            }
            printf("Child done!\n");
            close(fd1);
            close(fd2);
            break;
        }
        default: {
            // parent
            printf("Trying to open FIFO '%s' for writing\n", FIFO2_NAME);
            if ((fd2 = open(FIFO2_NAME, O_WRONLY)) < 0) {
                perror("open(): ");
                return 1;
            }
            // TODO: switch order here
            printf("Trying to open FIFO '%s' for writing\n", FIFO1_NAME);
            if ((fd1 = open(FIFO1_NAME, O_WRONLY)) < 0) {
                perror("open(): ");
                return 1;
            }
            printf("Parent done!\n");
            close(fd1);
            close(fd2);
            while (wait(NULL) > 0);
            break;
        }
    }

    return 0;
}
