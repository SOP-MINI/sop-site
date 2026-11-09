#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void sigint_handler(int signo) {
  printf("[%lu] Thread received SIGINT\n", pthread_self());
}

void do_stuff() {
  while (1) {
    printf("[%lu] Thread waiting for signal\n", pthread_self());
    pause();
  }
}

void *thread_work(void *arg) {
  printf("[%lu] Hello from a worker thread!\n", pthread_self());
  do_stuff();
  printf("[%lu] Bye from a worker thread!\n", pthread_self());
  return NULL;
}

#define WORKER_THREADS 4

int main() {

  struct sigaction sa;
  memset(&sa, 0, sizeof(struct sigaction));
  sa.sa_handler = sigint_handler;

  int err;

  if (sigaction(SIGINT, &sa, NULL) < 0) {
    perror("sigaction()");
    return 1;
  }

  pthread_t tid[WORKER_THREADS];

  for (int i = 0; i < WORKER_THREADS; ++i) {
    if ((err = pthread_create(&tid[i], NULL, thread_work, NULL)) != 0) {
      fprintf(stderr, "pthread_create(): %s\n", strerror(err));
      return 1;
    }
  }

  do_stuff();

  for (int i = 0; i < WORKER_THREADS; ++i) {
    if ((err = pthread_join(tid[i], NULL)) != 0) {
      fprintf(stderr, "pthread_join(): %s\n", strerror(err));
    }
  }

  return 0;
}
