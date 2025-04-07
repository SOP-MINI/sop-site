#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ERR(source) \
    (fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), perror(source), kill(0, SIGKILL), exit(EXIT_FAILURE))

int sethandler(void (*f)(int), int sigNo)
{
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = f;
    if (-1 == sigaction(sigNo, &act, NULL))
        return -1;
    return 0;
}

void sigchld_handler(int sig)
{
    pid_t pid;
    for (;;)
    {
        pid = waitpid(0, NULL, WNOHANG);
        if (0 == pid)
            return;
        if (0 >= pid)
        {
            if (ECHILD == errno)
                return;
            ERR("waitpid:");
        }
    }
}

void child_work(int fd, int R)
{
    srand(getpid());
    char c = 'a' + rand() % ('z' - 'a');
    if (write(R, &c, 1) < 0)
        ERR("write to R");
}

void parent_work(int n, int *fds, int R)
{
    char c;
    int status;
    srand(getpid());
    while ((status = read(R, &c, 1)) == 1)
        printf("%c", c);
    if (status < 0)
        ERR("read from R");
    printf("\n");
}

void create_children_and_pipes(int n, int *fds, int R)
{
    int tmpfd[2];
    int max = n;
    while (n)
    {
        if (pipe(tmpfd))
            ERR("pipe");
        switch (fork())
        {
            case 0:
                while (n < max)
                    if (fds[n] && close(fds[n++]))
                        ERR("close");
                free(fds);
                if (close(tmpfd[1]))
                    ERR("close");
                child_work(tmpfd[0], R);
                if (close(tmpfd[0]))
                    ERR("close");
                if (close(R))
                    ERR("close");
                exit(EXIT_SUCCESS);

            case -1:
                ERR("Fork:");
        }
        if (close(tmpfd[0]))
            ERR("close");
        fds[--n] = tmpfd[1];
    }
}

void usage(char *name)
{
    fprintf(stderr, "USAGE: %s n\n", name);
    fprintf(stderr, "0<n<=10 - number of children\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    int n, *fds, R[2];
    if (2 != argc)
        usage(argv[0]);
    n = atoi(argv[1]);
    if (n <= 0 || n > 10)
        usage(argv[0]);
    if (pipe(R))
        ERR("pipe");
    if (NULL == (fds = (int *)malloc(sizeof(int) * n)))
        ERR("malloc");
    if (sethandler(sigchld_handler, SIGCHLD))
        ERR("Seting parent SIGCHLD:");
    create_children_and_pipes(n, fds, R[1]);
    if (close(R[1]))
        ERR("close");
    parent_work(n, fds, R[0]);
    while (n--)
        if (fds[n] && close(fds[n]))
            ERR("close");
    if (close(R[0]))
        ERR("close");
    free(fds);
    return EXIT_SUCCESS;
}
