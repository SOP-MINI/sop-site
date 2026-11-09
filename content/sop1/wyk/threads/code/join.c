#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct thread_args {
  int idx;
  pthread_t prev_tid;
};

void *thread_work(void *arg) {
  struct thread_args* a = (struct thread_args *)arg;
  int err;

  int idx = a->idx;
  pthread_t prev_tid = a->prev_tid;
  free(arg);

  printf("[%lu] Hello from a worker thread!\n", pthread_self());

  int* val;

  if (idx == 0) {
    // First thread just returns
    val = (int*)malloc(sizeof(int));
    *val = 0;
  } else {
    void *ret;
    if ((err = pthread_join(prev_tid, &ret)) != 0) {
      fprintf(stderr, "pthread_join(): %s", strerror(err));
    }
    val = (int*)ret;
    ++(*val);
  }

  sleep(1);
  printf("[%lu] Returning %d!\n", pthread_self(), *val);

  return val;
}

int main() {

  int err;
  pthread_t prev_tid = 0;
  for (int i = 0; i < 3; ++i) {
    struct thread_args *arg = (struct thread_args *)malloc(sizeof(struct thread_args));
    arg->idx = i;
    arg->prev_tid = prev_tid;
    pthread_t tid;
    if ((err = pthread_create(&tid, NULL, thread_work, arg)) != 0) {
      fprintf(stderr, "pthread_create() failed: %s", strerror(err));
      return 1;
    }
    prev_tid = tid;
  }

  // some long computation

  void *ret;
  if ((err = pthread_join(prev_tid, &ret)) != 0) {
    fprintf(stderr, "pthread_join(): %s", strerror(err));
  }
  int a = *(int *)ret;
  printf("Thread [%lu] returned %d\n", prev_tid, a);
  free(ret);

  return 0;
}
