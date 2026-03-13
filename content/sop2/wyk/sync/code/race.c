#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM_ACCOUNTS 10
#define NUM_THREADS 2
#define ITERATIONS 10000000
#define INITIAL_BALANCE 1000

int balances[NUM_ACCOUNTS];

int transfer(int* balances, int src, int dst, int amount) {
    if (balances[src] < amount) {
        return -1;
    }
    balances[src] -= amount;
    balances[dst] += amount;
    return 0;
}

void *client_simulator(void *arg) {
    (void)arg;
    unsigned int seed = (unsigned int)pthread_self();

    for (int i = 0; i < ITERATIONS; ++i) {
        int src = rand_r(&seed) % NUM_ACCOUNTS;
        int dst = rand_r(&seed) % NUM_ACCOUNTS;

        while (src == dst) {
            dst = rand_r(&seed) % NUM_ACCOUNTS;
        }

        int amount = (rand_r(&seed) % 100) + 1;

        transfer(balances, src, dst, amount);
    }

    return NULL;
}

int total_balance() {
    int total = 0;
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        total += balances[i];
    }
    return total;
}

int main() {
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        balances[i] = INITIAL_BALANCE;
    }

    int initial_total = total_balance();
    printf("Initial total balance: %d\n", initial_total);

    pthread_t threads[NUM_THREADS];
    int ret;

    for (int i = 0; i < NUM_THREADS; i++) {
        if ((ret = pthread_create(&threads[i], NULL, client_simulator, NULL)) != 0) {
            fprintf(stderr, "pthread_create(): %s\n", strerror(ret));
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    int final_total = total_balance();
    printf("Final total balance: %d\n", final_total);

    return 0;
}