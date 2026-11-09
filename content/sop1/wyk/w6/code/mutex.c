#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct cbl {
  int counter;
  pthread_mutex_t mutex;
} cbl_t;

void do_stuff(cbl_t *c) {
  for (int i = 0; i < 1000000; ++i) {
    pthread_mutex_lock(&c->mutex);
    c->counter++;
    pthread_mutex_unlock(&c->mutex);
  }
}

void *thread_work(void *arg) {
  do_stuff((cbl_t *)arg);
  return NULL;
}

#define WORKER_THREADS 4

int main() {

  pthread_t tid[WORKER_THREADS];
  cbl_t c;
  c.counter = 0;

  if (pthread_mutex_init(&c.mutex, NULL) < 0) {
    perror("pthread_mutex_init()");
    return 1;
  }

  // pthread_mutex_t copy = c.mutex; // WRONG!

  for (int i = 0; i < WORKER_THREADS; ++i) {
    if (pthread_create(&tid[i], NULL, thread_work, &c) < 0) {
      perror("pthread_create()");
      return 1;
    }
  }

  do_stuff(&c);

  for (int i = 0; i < WORKER_THREADS; ++i) {
    if (pthread_join(tid[i], NULL) < 0) {
      perror("pthread_join()");
    }
  }

  printf("counter = %d\n", c.counter);

  return 0;
}
