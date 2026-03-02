#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define ITERATIONS 10000000

sem_t sem;

void lock(void) {
    sem_wait(&sem);
}

void unlock(void) {
    sem_post(&sem);
}

void *incrementer(void *arg) {
    int *counter = (int *) arg;

    for (int i = 0; i < ITERATIONS; ++i) {
        lock();
        (*counter)++;
        unlock();
    }

    return NULL;
}

void *decrementer(void *arg) {
    int *counter = (int *) arg;

    for (int i = 0; i < ITERATIONS; ++i) {
        lock();
        (*counter)--;
#ifdef PAUSE_IN_CS
        printf("pause()\n");
        // TODO: Enable and see CPU utilization
        pause();
#endif
        unlock();
    }

    return NULL;
}

int main() {

    sem_init(&sem, 0, 1);

    srand(getpid());

    int counter = 0;

    printf("Creating incrementer\n");

    pthread_t incrementer_tid, decrementer_tid;
    int ret;
    if ((ret = pthread_create(&incrementer_tid, NULL, incrementer, &counter)) != 0) {
        fprintf(stderr, "pthread_create(): %s", strerror(ret));
        return 1;
    }

    printf("Creating decrementer\n");

    if ((ret = pthread_create(&decrementer_tid, NULL, decrementer, &counter)) != 0) {
        fprintf(stderr, "pthread_create(): %s", strerror(ret));
        return 1;
    }

    printf("Created both threads\n");

    pthread_join(incrementer_tid, NULL);
    pthread_join(decrementer_tid, NULL);

    printf("counter = %d\n", counter);

    return 0;
}
