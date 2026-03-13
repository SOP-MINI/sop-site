#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>

pthread_barrier_t barrier;
int store1, store2;
int load1, load2;

void *th1(void *arg) {
    (void)arg;
    while (1) {
        pthread_barrier_wait(&barrier);

        /**
         * gcc -O3
         * mov     DWORD PTR store1[rip], 1
         * mov     eax, DWORD PTR store2[rip]
         * mov     DWORD PTR load1[rip], eax
         */
        atomic_store_explicit(&store1, 1, memory_order_relaxed);
        // TODO:
        // atomic_thread_fence(memory_order_seq_cst);
        load1 = atomic_load_explicit(&store2, memory_order_relaxed);

        pthread_barrier_wait(&barrier);
    }
}

void *th2(void *arg) {
    (void)arg;
    while (1) {
        pthread_barrier_wait(&barrier);

        /**
         * gcc -O3
         * mov     DWORD PTR store2[rip], 1
         * mov     eax, DWORD PTR store1[rip]
         * mov     DWORD PTR load2[rip], eax
         */
        atomic_store_explicit(&store2, 1, memory_order_relaxed);
        // TODO:
        // atomic_thread_fence(memory_order_seq_cst);
        load2 = atomic_load_explicit(&store1, memory_order_relaxed);

        pthread_barrier_wait(&barrier);
    }
}

int main() {

    pthread_barrier_init(&barrier, NULL, 3);

    printf("Creating thread 1\n");

    pthread_t producer_tid, consumer_tid;
    int ret;
    if ((ret = pthread_create(&producer_tid, NULL, th1, NULL)) != 0) {
        fprintf(stderr, "pthread_create(): %s", strerror(ret));
        return 1;
    }

    printf("Creating thread 2\n");

    if ((ret = pthread_create(&consumer_tid, NULL, th2, NULL)) != 0) {
        fprintf(stderr, "pthread_create(): %s", strerror(ret));
        return 1;
    }

    long iterations = 0;
    long reorders = 0;

    while (1) {
        store1 = store2 = 0;
        pthread_barrier_wait(&barrier); // Start both thread's work

        pthread_barrier_wait(&barrier); // Wait for both threads to finish

        iterations++;
        if (load1 == 0 && load2 == 0) {
            reorders++;
            fprintf(stderr, "reorder! (%ld reorders in %ld iterations - %.2f)\n",
                    reorders, iterations, 100.0 * reorders / (double)iterations);
        }
    }
}
