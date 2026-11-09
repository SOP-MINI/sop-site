#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *thread_work(void *arg) {

#ifdef NOCANCEL
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
#endif

  for (int i = 0; i < 5; ++i) {
    printf("[%ld] Working...\n", pthread_self());
    struct timespec ts = {1, 0};
    nanosleep(&ts, NULL);
  }
  return arg;
}

int main() {
  printf("[%lu] Hello from main()\n", pthread_self());

  int err;

  pthread_t tid;
  if ((err = pthread_create(&tid, NULL, thread_work, NULL)) != 0) {
    fprintf(stderr, "pthread_create(): %s\n", strerror(err));
    return 1;
  }

  getchar();

  if ((err = pthread_cancel(tid)) != 0) {
    fprintf(stderr, "pthread_cancel(): %s\n", strerror(err));
  }

  void *ret;
  if ((err = pthread_join(tid, &ret)) != 0) {
    fprintf(stderr, "pthread_join(): %s\n", strerror(err));
    return 1;
  }

  if (ret == PTHREAD_CANCELED) {
    printf("Thread was cancelled\n");
  } else {
    printf("Thread returned normally\n");
  }

  return 0;
}
