#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct worker_thread_args {
  int a;
  int b;
} worker_thread_args_t;

void *thread_work(void *arg) {
  worker_thread_args_t* args = (worker_thread_args_t *)arg;

  printf("[%lu] Hello from a worker thread! a = %d, b = %d\n", pthread_self(), args->a, args->b);
  sleep(2);
  printf("[%lu] Bye!\n", pthread_self());

  args->a += args->b;

  // TODO: What if address of a local variable was returned instead?
  return arg;
}

int main() {
  printf("[%lu] Hello from main()\n", pthread_self());

  pthread_t tid[3];
  worker_thread_args_t args[3];
  int err;
  for (int i = 0; i < 3; ++i) {
    // TODO: What if arg was stack allocated?
    args[i].a = i;
    args[i].b = 2 * i;
    if ((err = pthread_create(&tid[i], NULL, thread_work, &args[i])) != 0) {
      fprintf(stderr, "pthread_create() failed: %s", strerror(err));
      return 1;
    }
  }

  // some long computation

#ifdef NOJOIN
  return 0; // TODO: Run under valrgind without joining
#endif

  for (int i = 0; i < 3; ++i) {
    void *ret;
    if ((err = pthread_join(tid[i], &ret)) != 0) {
      fprintf(stderr, "pthread_join(): %s", strerror(err));
    }
    worker_thread_args_t rval = *(worker_thread_args_t *)ret;
    printf("Thread [%lu] returned (%d, %d)\n", tid[i], rval.a, rval.b);
    // free(ret);
  }

  return 0;
}
