#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define CPU_AFFINITY 2

void cpu_burst(int id, int scale) {
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  printf("[Th %d] CPU burst begins\n", id);

  volatile unsigned long long it, i;
  for (it = 0; it < scale; ++it) {
    for (i = 0; i < 10000000; i++) {
      if (i % 1000000 == 0) usleep(1);
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &end);
  double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
  printf("[Th %d] CPU burst ends after %.2fs\n", id, elapsed);
}

void* thread_function(void* arg) {
  int id = (int)(uintptr_t)arg;
  if (id == 0) {
    // Start priority thread with some delay
    sleep(1);
  }
  cpu_burst(id, 1000);
  return NULL;
}

int main() {
  pthread_t threads[3];
  pthread_attr_t attr[3];
  struct sched_param param;
  cpu_set_t cpuset;

  CPU_ZERO(&cpuset);
  CPU_SET(CPU_AFFINITY, &cpuset);

  for (int i = 0; i < 3; i++) {
    pthread_attr_init(&attr[i]);
    pthread_attr_setaffinity_np(&attr[i], sizeof(cpu_set_t), &cpuset);

    if (i == 0) {
      // Set real time priority for thread 0
      pthread_attr_setschedpolicy(&attr[i], SCHED_FIFO);
      param.sched_priority = 50; // Range 1-99
      pthread_attr_setschedparam(&attr[i], &param);
      pthread_attr_setinheritsched(&attr[i], PTHREAD_EXPLICIT_SCHED);
    }
  }

  for (int i = 0; i < 3; i++) {
    int ret;
    if ((ret = pthread_create(&threads[i], &attr[i], thread_function, (void*)(uintptr_t)i)) != 0) {
      fprintf(stderr, "pthread_create failed: %s\n", strerror(ret));
      return EXIT_FAILURE;
    }
  }

  for (int i = 0; i < 3; i++) pthread_join(threads[i], NULL);

  return 0;
}