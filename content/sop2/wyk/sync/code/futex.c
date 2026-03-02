#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <sys/syscall.h>
#include <linux/futex.h>

#define ITERATIONS 10000000
#define NUM_THREADS 4

typedef struct {
    atomic_int state;
} futex_mutex_t;

futex_mutex_t mtx = {0};

static int futex_call(atomic_int *uaddr, int futex_op, int val) {
    return syscall(SYS_futex, uaddr, futex_op, val, NULL, NULL, 0);
}

void lock(futex_mutex_t* m) {
    int expected = 0;
    if (atomic_compare_exchange_strong(&m->state, &expected, 1)) {
        return;
    }
    while (1) {
        if (atomic_exchange(&m->state, 2) != 0) {
            futex_call(&m->state, FUTEX_WAIT, 2);
        } else {
            return;
        }
    }
}

void unlock(futex_mutex_t* m) {
    if (atomic_fetch_sub(&m->state, 1) != 1) {
        atomic_store(&m->state, 0);
        futex_call(&m->state, FUTEX_WAKE, 1);
    }
}

void *incrementer(void *arg) {
    int *counter = (int *) arg;
    for (int i = 0; i < ITERATIONS; ++i) {
        lock(&mtx);
        (*counter)++;
        unlock(&mtx);
    }
    return NULL;
}

void *decrementer(void *arg) {
    int *counter = (int *) arg;
    for (int i = 0; i < ITERATIONS; ++i) {
        lock(&mtx);
        (*counter)--;
        unlock(&mtx);
    }
    return NULL;
}

int main()
{
    srand(getpid());
    int counter = 0;

    pthread_t incrementer_tids[NUM_THREADS];
    pthread_t decrementer_tids[NUM_THREADS];

    printf("Creating %d incrementers and %d decrementers\n", NUM_THREADS, NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&incrementer_tids[i], NULL, incrementer, &counter) != 0) {
            fprintf(stderr, "pthread_create failed\n");
            return 1;
        }
        if (pthread_create(&decrementer_tids[i], NULL, decrementer, &counter) != 0) {
            fprintf(stderr, "pthread_create failed\n");
            return 1;
        }
    }

    printf("Created all threads\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(incrementer_tids[i], NULL);
        pthread_join(decrementer_tids[i], NULL);
    }

    printf("counter = %d\n", counter);
    return 0;
}
