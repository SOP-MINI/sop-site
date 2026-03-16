#define _GNU_SOURCE // Enables pthread_setname_np()
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>

#define N 3
#define ITERATIONS 10000000

typedef struct context {
    int idx;
    sem_t *left;
    sem_t *right;
} context_t;

void *philosopher(void *arg) {
    context_t *ctx = (context_t *)arg;

    for (int i = 0; i < ITERATIONS; ++i) {
        sem_wait(ctx->left);
        sem_wait(ctx->right);

        printf("Philosopher %d eats\n", ctx->idx);

        struct timespec ts = {0, 50000000000};
        nanosleep(&ts, NULL);

        sem_post(ctx->right);
        sem_post(ctx->left);

        printf("Philosopher %d thinks\n", ctx->idx);

        nanosleep(&ts, NULL);
    }

    return NULL;
}

int main() {
    int ret;
    sem_t chopsticks[N];
    pthread_t tids[N];
    context_t ctxs[N];

    for (int i = 0; i < N; ++i) {
        sem_init(&chopsticks[i], 0, 1);
        ctxs[i].idx = i;
        ctxs[i].left = &chopsticks[i];
        ctxs[i].right = &chopsticks[(i + 1) % N];
    }

    printf("Creating philosophers\n");

    for (int i = 0; i < N; ++i) {
        if ((ret = pthread_create(&tids[i], NULL, philosopher, &ctxs[i])) != 0) {
            fprintf(stderr, "pthread_create(): %s", strerror(ret));
            return 1;
        }
        char name[16];
        snprintf(name, sizeof(name), "philosopher %d", i);
        pthread_setname_np(tids[i], name);
    }

    printf("Joining all threads\n");

    for (int i = 0; i < N; ++i) {
        pthread_join(tids[i], NULL);
    }

    for (int i = 0; i < N; ++i) {
        sem_destroy(&chopsticks[i]);
    }

    return 0;
}
