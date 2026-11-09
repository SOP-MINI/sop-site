#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define ITERATIONS 10000000

void *poster(void *arg) {
    sem_t *sem = (sem_t *) arg;

    while (1) {
      getchar();
      printf("calling sem_post()\n");
      sem_post(sem);
    }

    return NULL;
}

void *waiter(void *arg) {
    sem_t *sem = (sem_t *) arg;

    while(sem_wait(sem) == 0) {
      printf("sem_wait() returned\n");
    }

    return NULL;
}

int main() {
    sem_t sem;
    sem_init(&sem, 0, 5);

    printf("Creating poster\n");

    pthread_t poster_tid, waiter_tid;
    int ret;
    if ((ret = pthread_create(&poster_tid, NULL, poster, &sem)) != 0) {
        fprintf(stderr, "pthread_create(): %s", strerror(ret));
        return 1;
    }

    printf("Creating waiter\n");

    if ((ret = pthread_create(&waiter_tid, NULL, waiter, &sem)) != 0) {
        fprintf(stderr, "pthread_create(): %s", strerror(ret));
        return 1;
    }

    printf("Created both threads\n");

    pthread_join(poster_tid, NULL);
    pthread_join(waiter_tid, NULL);

    return 0;
}
