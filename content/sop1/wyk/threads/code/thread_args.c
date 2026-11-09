#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *thread_work(void *arg) {
  printf("Hello from a worker thread! (arg = %d)\n", *(int *)arg);
  sleep(2);
  printf("Bye!\n");
  int *x = (int *)malloc(sizeof(int));
  *x = 2 * (*(int *)arg);
  return x;
}

int main() {
  int err;
  pthread_t tid[3];
  int arg[3] = {1, 3, 5};

  for (int i = 0; i < 3; ++i) {
    if ((err = pthread_create(&tid[i], NULL, thread_work, &arg[i])) != 0) {
      fprintf(stderr, "pthread_create(): %s", strerror(err));
    }
  }

  for (int i = 0; i < 3; ++i) {
    void *ret;
    if ((err = pthread_join(tid[i], &ret)) != 0) {
      fprintf(stderr, "pthread_join(): %s", strerror(err));
    }

    printf("thread returned %d\n", *(int *)ret);
    free(ret);
  }

  return 0;
}
