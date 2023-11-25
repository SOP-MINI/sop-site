#define _GNU_SOURCE
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

#define FS_NUM 5
#define MAX_INPUT 120
volatile sig_atomic_t do_work = 1;

void sigint_handler(int sig) { do_work = 0; }

int sethandler(void (*f)(int), int sigNo)
{
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = f;
    if (-1 == sigaction(sigNo, &act, NULL))
        return -1;
    return 0;
}

struct arguments
{
    int32_t time;
    sem_t *semaphore;
};

void *thread_func(void *arg)
{
    struct arguments *args = (struct arguments *)arg;
    uint32_t tt;
    fprintf(stderr, "Will sleep for %d\n", args->time);
    for (tt = args->time; tt > 0; tt = sleep(tt))
        ;
    puts("Wake up");
    if (sem_post(args->semaphore) == -1)
        ERR("sem_post");
    free(args);
    return NULL;
}

void doWork()
{
    int32_t time;
    pthread_t thread;
    char input[MAX_INPUT];
    struct arguments *args;
    sem_t semaphore;
    if (sem_init(&semaphore, 0, FS_NUM) != 0)
        ERR("sem_init");
    while (do_work)
    {
        puts("Please enter the number of seconds for the alarm delay:");
        if(fgets(input, MAX_INPUT, stdin) == NULL) {
            if (errno == EINTR)
                continue;
            ERR("fgets:");
        }
        
        time = atoi(input);
        if(time < 0) {
            fputs("Incorrect time specified", stderr);
            continue;
        }
        
        if (TEMP_FAILURE_RETRY(sem_trywait(&semaphore)) == -1)
        {
            switch (errno)
            {
                case EAGAIN:
                    fprintf(stderr, "Only %d alarms can be set at the time.", FS_NUM);
                case EINTR:
                    continue;
            }
            ERR("sem_trywait");
        }
        
        if ((args = (struct arguments *)malloc(sizeof(struct arguments))) == NULL)
            ERR("malloc:");
        args->time = time;
        args->semaphore = &semaphore;
        if (pthread_create(&thread, NULL, thread_func, (void *) args) != 0)
            ERR("pthread_create");
        if (pthread_detach(thread) != 0)
            ERR("pthread_detach");
    }
}

int main(int argc, char **argv)
{
    if (sethandler(sigint_handler, SIGINT))
        ERR("Seting SIGINT:");
    doWork();
    fprintf(stderr, "Program has terminated.\n");
    return EXIT_SUCCESS;
}
