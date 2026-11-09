#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *thread_work(void *arg) {
  char *line = arg;

  printf("[%lu] Processing line '%s' starts\n", pthread_self(), line);
  sleep(2);
  printf("[%lu] Processing line '%s' ends\n", pthread_self(), line);

  free(arg);
  return arg;
}

int main() {
  printf("[%lu] Hello from main()\n", pthread_self());

  char line[32];
  while (scanf("%31s", line) > 0) {

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_t tid;
    if (pthread_create(&tid, &attr, thread_work, strdup(line)) < 0) {
      perror("pthread_create()");
      return 1;
    }

    pthread_attr_destroy(&attr);

    // Equivalent: pthread_detach(tid);
  }

  return 0;
}
