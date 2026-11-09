#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void demonize() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    printf("child process PID: %d, SID: %d\n", getpid(), getsid(0));

    pid_t sid = setsid();
    if (sid < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }

    if (chdir("/") < 0) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);
}

int main() {
    printf("launcher process PID: %d, SID: %d\n", getpid(), getsid(0));

    demonize();

    FILE *fp = fopen("/tmp/demon.log", "w");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "demon running PID: %d, PPID: %d, SID: %d\n", getpid(), getppid(), getsid(0));

    int counter = 0;
    while (counter < 10) {
        fprintf(fp, "demon works, counter: %d\n", counter++);
        fflush(fp);
        sleep(1);
    }

    fprintf(fp, "demon exiting PID: %d, SID: %d\n", getpid(), getsid(0));
    fclose(fp);

    return EXIT_SUCCESS;
}