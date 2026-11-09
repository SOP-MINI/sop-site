#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void free_buffer(void *buff) {
  printf("Freeing buffer\n");
  free(buff); // void free(void*)
}

struct args {
  int counter;
  int i;
};

void *thread_work(void *arg) {

#ifdef ASYNC
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
#endif

  struct args* a = (struct args*)arg;

  void *buffer = malloc(1000);

  pthread_cleanup_push(free_buffer, buffer);

  for (a->i = 0; a->i < 1000000; ++a->i) {
    for (a->counter = 0; a->counter < 10000; ++a->counter) {
    }
    pthread_testcancel();
  }

  pthread_cleanup_pop(1); // free(buffer)

  printf("Thread exiting\n");
  return NULL;
}

int main() {
  printf("[%lu] Hello from main()\n", pthread_self());

  struct args a;
  memset(&a, 0, sizeof(a));

  int err;
  pthread_t tid;

  if ((err = pthread_create(&tid, NULL, thread_work, &a)) != 0) {
    fprintf(stderr, "pthread_create(): %s\n", strerror(err));
    return 1;
  }

  getchar();

  printf("Cancelling thread\n");
  if ((err = pthread_cancel(tid)) != 0) {
    fprintf(stderr, "pthread_cancel(): %s\n", strerror(err));
  }

  if ((err = pthread_join(tid, NULL)) != 0) {
    fprintf(stderr, "pthread_join(): %s\n", strerror(err));
    return 1;
  }

  printf("counter = %d, i = %d\n", a.counter, a.i);

  return 0;
}
