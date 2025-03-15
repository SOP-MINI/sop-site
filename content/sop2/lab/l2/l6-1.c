#define _GNU_SOURCE
#include <errno.h>
#include <mqueue.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define LIFE_SPAN 10
#define MAX_NUM 10

#define ERR(source) \
    (fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), perror(source), kill(0, SIGKILL), exit(EXIT_FAILURE))

volatile sig_atomic_t children_left = 0;

void sethandler(void (*f)(int, siginfo_t *, void *), int sigNo)
{
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_sigaction = f;
    act.sa_flags = SA_SIGINFO;
    if (-1 == sigaction(sigNo, &act, NULL))
        ERR("sigaction");
}

void mq_handler(int sig, siginfo_t *info, void *p)
{
    mqd_t *pin;
    uint8_t ni;
    unsigned msg_prio;

    pin = (mqd_t *)info->si_value.sival_ptr;

    static struct sigevent not ;
    not .sigev_notify = SIGEV_SIGNAL;
    not .sigev_signo = SIGRTMIN;
    not .sigev_value.sival_ptr = pin;
    if (mq_notify(*pin, &not ) < 0)
        ERR("mq_notify");

    for (;;)
    {
        if (mq_receive(*pin, (char *)&ni, 1, &msg_prio) < 1)
        {
            if (errno == EAGAIN)
                break;
            else
                ERR("mq_receive");
        }
        if (0 == msg_prio)
            printf("MQ: got timeout from %d.\n", ni);
        else
            printf("MQ: %d is a bingo number!\n", ni);
    }
}

void sigchld_handler(int sig, siginfo_t *s, void *p)
{
    pid_t pid;
    for (;;)
    {
        pid = waitpid(0, NULL, WNOHANG);
        if (pid == 0)
            return;
        if (pid <= 0)
        {
            if (errno == ECHILD)
                return;
            ERR("waitpid");
        }
        children_left--;
    }
}

void child_work(int n, mqd_t pin, mqd_t pout)
{
    srand(getpid());
    uint8_t my_bingo = (uint8_t)(rand() % MAX_NUM);
    uint8_t ni;
    for (int life = rand() % LIFE_SPAN + 1; life > 0; life--)
    {
        if (TEMP_FAILURE_RETRY(mq_receive(pout, (char *)&ni, 1, NULL)) < 1)
            ERR("mq_receive");
        printf("[%d] Received %d\n", getpid(), ni);
        if (my_bingo == ni)
        {
            if (TEMP_FAILURE_RETRY(mq_send(pin, (const char *)&my_bingo, 1, 1)))
                ERR("mq_send");
            return;
        }
    }
    if (TEMP_FAILURE_RETRY(mq_send(pin, (const char *)&n, 1, 0)))
        ERR("mq_send");
}

void parent_work(mqd_t pout)
{
    srand(getpid());
    while (children_left)
    {
        uint8_t ni = (uint8_t)(rand() % MAX_NUM);
        if (TEMP_FAILURE_RETRY(mq_send(pout, (const char *)&ni, 1, 0)))
            ERR("mq_send");
        sleep(1);
    }
    printf("[PARENT] Terminates \n");
}

void create_children(int n, mqd_t pin, mqd_t pout)
{
    while (n > 0)
    {
        switch (fork())
        {
            case 0:
                child_work(n, pin, pout);
                exit(EXIT_SUCCESS);
            case -1:
                ERR("fork");
        }
        children_left++;
        n--;
    }
}

void usage(char *name)
{
    fprintf(stderr, "USAGE: %s n k p l\n", name);
    fprintf(stderr, "100 > n > 0 - number of children\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    int n;
    if (argc != 2)
        usage(argv[0]);
    n = atoi(argv[1]);
    if (n <= 0 || n >= 100)
        usage(argv[0]);

    mqd_t pin, pout;
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1;
    if ((pin = TEMP_FAILURE_RETRY(mq_open("/bingo_in", O_RDWR | O_NONBLOCK | O_CREAT, 0600, &attr))) == (mqd_t)-1)
        ERR("mq_open in");
    if ((pout = TEMP_FAILURE_RETRY(mq_open("/bingo_out", O_RDWR | O_CREAT, 0600, &attr))) == (mqd_t)-1)
        ERR("mq_open out");

    sethandler(sigchld_handler, SIGCHLD);
    sethandler(mq_handler, SIGRTMIN);
    create_children(n, pin, pout);

    static struct sigevent noti;
    noti.sigev_notify = SIGEV_SIGNAL;
    noti.sigev_signo = SIGRTMIN;
    noti.sigev_value.sival_ptr = &pin;
    if (mq_notify(pin, &noti) < 0)
        ERR("mq_notify");

    parent_work(pout);

    mq_close(pin);
    mq_close(pout);
    if (mq_unlink("/bingo_in"))
        ERR("mq unlink");
    if (mq_unlink("/bingo_out"))
        ERR("mq unlink");
    return EXIT_SUCCESS;
}
