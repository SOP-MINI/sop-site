#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define IS_CHILD 0
#define ERR(source) \
    (fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), perror(source), kill(0, SIGKILL), exit(EXIT_FAILURE))

void child_work(int i)
{
    srand(time(NULL) * getpid());
    int t = 5 + rand() % (10 - 5 + 1);
    sleep(t);
    printf("PROCESS with pid %d terminates\n", getpid());
}

void create_children(int n)
{
    pid_t s;
    for (int i = 0; i < n; ++i)
    {
        if ((s = fork()) < 0)
        {
            ERR("Fork:");
        }

        if (s == IS_CHILD)
        {
            child_work(n);
            exit(EXIT_SUCCESS);
        }
    }
}

void usage(char *name)
{
    fprintf(stderr, "USAGE: %s 0<n\n", name);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        usage(argv[0]);
    }

    int n = atoi(argv[1]);
    if (n <= 0)
    {
        usage(argv[0]);
    }

    create_children(n);
    while (n > 0)
    {
        sleep(3);
        pid_t pid;
        for (;;)
        {
            pid = waitpid(0, NULL, WNOHANG);
            if (pid > 0)
                n--;
            if (0 == pid)
                break;
            if (0 >= pid)
            {
                if (ECHILD == errno)
                    break;
                ERR("waitpid:");
            }
        }
        printf("PARENT: %d processes remain\n", n);
    }
    return EXIT_SUCCESS;
}
