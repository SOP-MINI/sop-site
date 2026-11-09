#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define ITERATIONS 20

typedef struct context {
  int flag;
  pthread_mutex_t mtx; //!< Protects flag
  pthread_cond_t cv; //!< Signalled when flag changes
} context_t;

void *producer(void *arg) {
  context_t *ctx = (context_t *)arg;

  getchar();

  printf("setting flag = 1\n");
  pthread_mutex_lock(&ctx->mtx);
  ctx->flag++;
  pthread_cond_broadcast(&ctx->cv);
  pthread_mutex_unlock(&ctx->mtx);

  return NULL;
}

void *consumer(void *arg) {
  context_t *ctx = (context_t *)arg;

  pthread_mutex_lock(&ctx->mtx);
  while (ctx->flag == 0) {
    printf("calling pthread_cond_wait()\n");
    pthread_cond_wait(&ctx->cv, &ctx->mtx);
    printf("pthread_cond_wait() returned\n");
  }

  ctx->flag--;
  pthread_mutex_unlock(&ctx->mtx);

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
  if ((err = pthread_cond_init(&ctx.cv, NULL)) != 0) {
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
  pthread_cond_destroy(&ctx.cv);

  return 0;
}
