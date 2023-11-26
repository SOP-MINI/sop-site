#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

#define BUFFERSIZE 256
#define READCHUNKS 4
#define THREAD_NUM 3
volatile sig_atomic_t work = 1;

typedef struct
{
    int id;
    int *idlethreads;
    int *condition;
    pthread_cond_t *cond;
    pthread_mutex_t *mutex;
} thread_arg;

void sigint_handler(int sig) { work = 0; }

void set_handler(void (*f)(int), int sigNo)
{
    struct sigaction act;
    memset(&act, 0x00, sizeof(struct sigaction));
    act.sa_handler = f;
    
    if (-1 == sigaction(sigNo, &act, NULL))
        ERR("sigaction");
}

ssize_t bulk_read(int fd, char *buf, size_t count)
{
    int c;
    size_t len = 0;
    do
    {
        c = TEMP_FAILURE_RETRY(read(fd, buf, count));
        if (c < 0)
            return c;
        if (c == 0)
            return len;
        buf += c;
        len += c;
        count -= c;
    } while (count > 0);
    return len;
}

ssize_t bulk_write(int fd, char *buf, size_t count)
{
    int c;
    size_t len = 0;
    do
    {
        c = TEMP_FAILURE_RETRY(write(fd, buf, count));
        if (c < 0)
            return c;
        buf += c;
        len += c;
        count -= c;
    } while (count > 0);
    return len;
}

void cleanup(void *arg) { pthread_mutex_unlock((pthread_mutex_t *)arg); }

void read_random(int thread_id)
{
    char file_name[20];
    char buffer[BUFFERSIZE];
    snprintf(file_name, sizeof(file_name), "random%d.bin", thread_id);
    printf("Writing to a file %s\n", file_name);
    int i, in, out;
    ssize_t count;
    if ((out = TEMP_FAILURE_RETRY(open(file_name, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0777))) < 0)
        ERR("open");
    if ((in = TEMP_FAILURE_RETRY(open("/dev/urandom", O_RDONLY))) < 0)
        ERR("open");
    for (i = 0; i < READCHUNKS; i++)
    {
        if ((count = bulk_read(in, buffer, BUFFERSIZE)) < 0)
            ERR("bulk_read");
        if ((count = bulk_write(out, buffer, count)) < 0)
            ERR("bulk_write");
        sleep(1);
    }
    if (TEMP_FAILURE_RETRY(close(in)))
        ERR("close");
    if (TEMP_FAILURE_RETRY(close(out)))
        ERR("close");
}

void *thread_func(void *arg)
{
    thread_arg targ;
    memcpy(&targ, arg, sizeof(targ));
    while (1)
    {
        pthread_cleanup_push(cleanup, (void *)targ.mutex);
        if (pthread_mutex_lock(targ.mutex) != 0)
            ERR("pthread_mutex_lock");
        (*targ.idlethreads)++;
        while (!*targ.condition && work)
            if (pthread_cond_wait(targ.cond, targ.mutex) != 0)
                ERR("pthread_cond_wait");
        *targ.condition = 0;
        if (!work)
            pthread_exit(NULL);
        (*targ.idlethreads)--;
        pthread_cleanup_pop(1);
        read_random(targ.id);
    }
    return NULL;
}

void init(pthread_t *thread, thread_arg *targ, pthread_cond_t *cond, pthread_mutex_t *mutex, int *idlethreads,
          int *condition)
{
    int i;
    for (i = 0; i < THREAD_NUM; i++)
    {
        targ[i].id = i + 1;
        targ[i].cond = cond;
        targ[i].mutex = mutex;
        targ[i].idlethreads = idlethreads;
        targ[i].condition = condition;
        if (pthread_create(&thread[i], NULL, thread_func, (void *) &targ[i]) != 0)
            ERR("pthread_create");
    }
}

void do_work(pthread_cond_t *cond, pthread_mutex_t *mutex, const int *idlethreads, int *condition)
{
    char buffer[BUFFERSIZE];
    while (work)
    {
        if (fgets(buffer, BUFFERSIZE, stdin) != NULL)
        {
            if (pthread_mutex_lock(mutex) != 0)
                ERR("pthread_mutex_lock");
            if (*idlethreads == 0)
            {
                if (pthread_mutex_unlock(mutex) != 0)
                    ERR("pthread_mutex_unlock");
                fputs("No threads available\n", stderr);
            }
            else
            {
                if (pthread_mutex_unlock(mutex) != 0)
                    ERR("pthread_mutex_unlock");
                *condition = 1;
                if (pthread_cond_signal(cond) != 0)
                    ERR("pthread_cond_signal");
            }
        }
        else
        {
            if (EINTR == errno)
                continue;
            ERR("fgets");
        }
    }
}

int main(int argc, char **argv)
{
    int i, condition = 0, idlethreads = 0;
    pthread_t thread[THREAD_NUM];
    thread_arg targ[THREAD_NUM];
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    set_handler(sigint_handler, SIGINT);
    init(thread, targ, &cond, &mutex, &idlethreads, &condition);
    do_work(&cond, &mutex, &idlethreads, &condition);
    if (pthread_cond_broadcast(&cond) != 0)
        ERR("pthread_cond_broadcast");
    for (i = 0; i < THREAD_NUM; i++)
        if (pthread_join(thread[i], NULL) != 0)
            ERR("pthread_join");
    return EXIT_SUCCESS;
}
