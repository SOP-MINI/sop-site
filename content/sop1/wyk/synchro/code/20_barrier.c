#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAP_SIZE 10
#define ITERATIONS 1000

typedef struct context {
  pthread_barrier_t* barrier;
  int idx;
  unsigned char (*map)[MAP_SIZE];
  int* sums;
} context_t;

void do_horizontal(context_t* ctx);
void do_vertical(context_t* ctx);
void do_verify(context_t* ctx);

void* worker(void *arg) {
  context_t* ctx = (context_t*)arg;

  for (int i = 0; i < ITERATIONS; ++i) {
    do_horizontal(ctx);
    pthread_barrier_wait(ctx->barrier);
    do_vertical(ctx);
    int ret = pthread_barrier_wait(ctx->barrier);
    if (ret == PTHREAD_BARRIER_SERIAL_THREAD) {
      do_verify(ctx);
    }
    pthread_barrier_wait(ctx->barrier);
  }

  free(ctx);
  return NULL;
}

void do_horizontal(context_t* ctx) {
  unsigned char c = rand() % 256;
  for (int i = 0; i < MAP_SIZE; ++i) {
    // Fill rows
    ctx->map[ctx->idx][i] = c;
  }
}

void do_vertical(context_t* ctx) {
  ctx->sums[ctx->idx] = 0;
  for (int i = 0; i < MAP_SIZE; ++i) {
    // Sum columns
    ctx->sums[ctx->idx] += ctx->map[i][ctx->idx];
  }
}

void do_verify(context_t* ctx) {
  for (int i = 0; i < MAP_SIZE; ++i) {
    if (ctx->sums[i] != ctx->sums[0]) {
      printf("error: sums[%d] = %d, sums[%d] = %d\n", i, ctx->sums[i], 0, ctx->sums[0]);
      return;
    }
  }
  printf("ok!\n");
}

int main() {

  srand(getpid());

  unsigned char map[MAP_SIZE][MAP_SIZE];
  int sums[MAP_SIZE];
  pthread_barrier_t barrier;
  memset(map, 0, sizeof(map));

  int err;
  printf("Creating mutex and CVs...\n");
  if ((err = pthread_barrier_init(&barrier, NULL, MAP_SIZE)) != 0) {
    fprintf(stderr, "pthread_mutex_init(): %s", strerror(err));
    return 1;
  }

  printf("Creating workers\n");

  pthread_t tids[MAP_SIZE];
  for (int i = 0; i < MAP_SIZE; ++i) {
    int ret;
    context_t* ctx = (context_t*)malloc(sizeof(context_t));
    ctx->barrier = &barrier;
    ctx->map = map;
    ctx->sums = sums;
    ctx->idx = i;
    if ((ret = pthread_create(&tids[i], NULL, worker, ctx)) != 0) {
      fprintf(stderr, "pthread_create(): %s", strerror(ret));
      free(ctx);
      return 1;
    }
  }

  for (int i = 0; i < MAP_SIZE; ++i) {
    pthread_join(tids[i], NULL);
  }

  pthread_barrier_destroy(&barrier);

  return 0;
}
