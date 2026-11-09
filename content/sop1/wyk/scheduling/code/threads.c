#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>

// #define CPU_AFFINITY 2

void cpu_burst(int thread_id, int scale) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    printf("[Th %d] CPU burst begins\n", thread_id);

    volatile unsigned long long it, i;
    for (it = 0; it < scale; ++it) {
        for (i = 0; i < 10000000; i++);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("[Th %d] CPU burst ends after %.2fs\n", thread_id, elapsed);
}

void* thread_function(void* arg) {
    int id = *(int*)arg;
    free(arg);

    for (int i = 0; i < 3; i++) {
        cpu_burst(id, 1000);

        printf("[Th %d] Goes asleep for 1s...\n", id);
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t threads[3];
    pthread_attr_t attr;
    cpu_set_t cpuset;

    pthread_attr_init(&attr);

#ifdef CPU_AFFINITY
    CPU_ZERO(&cpuset);
    CPU_SET(CPU_AFFINITY, &cpuset);
    pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
#endif

    for (int i = 0; i < 3; i++) {
        int* id = (int*)malloc(sizeof(int));
        assert(id != NULL);
        *id = i;
        pthread_create(&threads[i], &attr, thread_function, id);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_attr_destroy(&attr);
    return 0;
}
