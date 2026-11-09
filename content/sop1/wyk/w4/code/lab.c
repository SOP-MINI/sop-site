// Lecture coded example of how to create process tree

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void create_children(int d) {
    printf("[pid=%d] create_children(%d) called\n", getpid(), d);

    if (d == 0) {
        // do_work()
        srand(getpid());
        sleep(1);
        int val = rand() % 3;
        printf("[pid=%d] generated %d\n", getpid(), val);
        exit(val);
    }

    pid_t pid1 = fork();
    switch (pid1) {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            // child
            create_children(d - 1);
            exit(EXIT_SUCCESS);
        default:
            break;
    }

    pid_t pid2 = fork();
    switch (pid2) {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            // child
            create_children(d - 1);
            exit(EXIT_SUCCESS);
        default:
            break;
    }

    int sum = 0;

    for (int i = 0; i < 2; ++i) {
        int status;
        if (wait(&status) < 0) {
            perror("wait");
        }
        if (WIFEXITED(status)) {
            if (sum != 255)
                sum += WEXITSTATUS(status);
        } else {
            sum = 255;
        }
    }

    printf("[pid=%d] returns sum = %d\n", getpid(), sum);
    exit(sum);
}

int main(int argc, char *argv[]) {

    int d = atoi(argv[1]);

    create_children(d);

    return 0;
}
