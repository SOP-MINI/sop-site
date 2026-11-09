#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void do_stuff(int *p_counter) {
  for (int i = 0; i < 1000000; ++i)
    (*p_counter)++;
}

void *thread_work(void *arg) {
  printf("[%lu] Hello from a worker thread!\n", pthread_self());
  do_stuff((int *)arg);
  printf("[%lu] Bye from a worker thread!\n", pthread_self());
  return NULL;
}

#define WORKER_THREADS 4

int main() {
  int err;
  pthread_t tid[WORKER_THREADS];
  int counter = 0;

  for (int i = 0; i < WORKER_THREADS; ++i) {
    if ((err = pthread_create(&tid[i], NULL, thread_work, &counter)) != 0) {
      fprintf(stderr, "pthread_create(): %s", strerror(err));
    }
  }

  for (int i = 0; i < WORKER_THREADS; ++i) {
    if ((err = pthread_join(tid[i], NULL)) != 0) {
      fprintf(stderr, "pthread_join(): %s", strerror(err));
    }
  }

  printf("counter = %d\n", counter);

  return 0;
}
