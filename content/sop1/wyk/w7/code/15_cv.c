#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>

#define BUFFER_SIZE 10
#define ITERATIONS 20

typedef struct context {
    pthread_mutex_t mtx;
    pthread_cond_t empty;
    pthread_cond_t nonempty;

    int items[BUFFER_SIZE];
    int in;
    int out;
    int size;
} context_t;


void* producer(void *arg) {
    context_t* ctx = (context_t*)arg;

    for (int i = 0; i < ITERATIONS; ++i) {
//        struct timespec ts = {0, (rand() % 1000) * 1000000};
//        nanosleep(&ts, NULL);

        pthread_mutex_lock(&ctx->mtx);
        while (ctx->size >= BUFFER_SIZE)
            pthread_cond_wait(&ctx->empty, &ctx->mtx);

        int new_item = i;
        ctx->items[ctx->in] = new_item;
        ctx->in = (ctx->in + 1) % BUFFER_SIZE;
        ctx->size++;

      pthread_cond_signal(&ctx->nonempty);
      pthread_mutex_unlock(&ctx->mtx);

      printf("produced '%d'\n", new_item);
    }

    return NULL;
}

void* consumer(void *arg) {
    context_t* ctx = (context_t*)arg;

    for (int i = 0; i < ITERATIONS; ++i) {
        pthread_mutex_lock(&ctx->mtx);
        while (ctx->size == 0)
            pthread_cond_wait(&ctx->nonempty, &ctx->mtx);

        int item = ctx->items[ctx->out];
        ctx->out = (ctx->out + 1) % BUFFER_SIZE;
        ctx->size--;

        pthread_cond_signal(&ctx->empty);
        pthread_mutex_unlock(&ctx->mtx);

        printf("consumed '%d'\n", item);
    }

    return NULL;
}

int main() {

    srand(getpid());

    context_t ctx;
    memset(&ctx, 0, sizeof(context_t));

    int err;
    printf("Creating mutex and CVs...\n");
    if ((err = pthread_mutex_init(&ctx.mtx, NULL)) != 0) {
        fprintf(stderr, "pthread_mutex_init(): %s", strerror(err));
        return 1;
    }
    if ((err = pthread_cond_init(&ctx.empty, NULL)) != 0) {
        fprintf(stderr, "pthread_mutex_init(): %s", strerror(err));
        return 1;
    }
    if ((err = pthread_cond_init(&ctx.nonempty, NULL)) != 0) {
        fprintf(stderr, "pthread_mutex_init(): %s", strerror(err));
        return 1;
    }


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

    pthread_mutex_destroy(&ctx.mtx);
    pthread_cond_destroy(&ctx.empty);
    pthread_cond_destroy(&ctx.nonempty);

    return 0;
}
