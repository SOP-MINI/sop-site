#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *thread_work(void *arg) {
  int a = *(int *)arg;

  printf("[%lu] Hello from a worker thread! a = %d, &a = %p\n", pthread_self(), a, &a);
  sleep(2);
  printf("[%lu] Bye!\n", pthread_self());

  // TODO: What if address of a local variable was returned instead?
  return arg;
}

int main() {
  printf("[%lu] Hello from main()\n", pthread_self());

  pthread_t tid[3];
  int err;
  for (int i = 0; i < 3; ++i) {
    // TODO: What if arg was stack allocated?
    int *arg = (int *)malloc(sizeof(int));
    (*arg) = i;
    if ((err = pthread_create(&tid[i], NULL, thread_work, arg)) != 0) {
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
    int a = *(int *)ret;
    printf("Thread [%lu] returned %d\n", tid[i], a);
    free(ret);
  }

  return 0;
}
