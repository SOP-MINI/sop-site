#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define ITERATIONS 10000000

typedef struct context {
    int *counter;
    sem_t *sem;
} context_t;

void *producer(void *arg) {
    context_t *ctx = (context_t *) arg;

    for (int i = 0; i < ITERATIONS; ++i) {
        sem_wait(ctx->sem);
        (*ctx->counter)++;
        sem_post(ctx->sem);
    }

    return NULL;
}

void *consumer(void *arg) {
    context_t *ctx = (context_t *) arg;

    for (int i = 0; i < ITERATIONS; ++i) {
        sem_wait(ctx->sem);
        (*ctx->counter)--;
        sem_post(ctx->sem);
    }

    return NULL;
}

int main() {

    sem_t *sem = sem_open("/sop_sem", O_CREAT | O_EXCL, 0600, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open()");
        return 1;
    }

    int counter = 0;

    context_t ctx = {&counter, sem};

    srand(getpid());

    printf("Creating producer\n");

    pthread_t producer_tid, consumer_tid;
    int ret;
    if ((ret = pthread_create(&producer_tid, NULL, producer, &ctx)) != 0) {
        fprintf(stderr, "pthread_create(): %s", strerror(ret));
        return 1;
    }

    printf("Creating consumer\n");

    if ((ret = pthread_create(&consumer_tid, NULL, consumer, &ctx)) != 0) {
        fprintf(stderr, "pthread_create(): %s", strerror(ret));
        return 1;
    }

    printf("Created both threads\n");

    pthread_join(producer_tid, NULL);
    pthread_join(consumer_tid, NULL);

    sem_close(sem);

    printf("counter = %d\n", counter);

    return 0;
}
