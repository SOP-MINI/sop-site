#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_ACCOUNTS 10
#define ITERATIONS 10000000
#define INITIAL_BALANCE 1000

int balances[NUM_ACCOUNTS];
atomic_int lck = 0;

/* gcc -02
lock:
        mov     eax, 1
.L2:
        xchg    eax, DWORD PTR lck[rip]
        test    eax, eax
        jne     .L2
        ret
 */
void lock(void) {
    int key = 1;
    do {
        key = atomic_exchange(&lck, key);
    } while(key);
}

/* gcc -02
unlock:
        xor     eax, eax
        xchg    eax, DWORD PTR lck[rip]
        ret
 */
void unlock(void) {
    lck = 0;
}

int transfer(int* balances, int src, int dst, int amount) {
    if (balances[src] < amount) {
        return -1;
    }
    balances[src] -= amount;
    balances[dst] += amount;
    return 0;
}

void *client_simulator(void *arg) {
    int i = *(int *) arg;

    unsigned int seed = (unsigned int)pthread_self();

    for (int j = 0; j < ITERATIONS; ++j) {
        int src = rand_r(&seed) % NUM_ACCOUNTS;
        int dst = rand_r(&seed) % NUM_ACCOUNTS;

        while (src == dst) {
            dst = rand_r(&seed) % NUM_ACCOUNTS;
        }

        int amount = (rand_r(&seed) % 100) + 1;

        lock();
        transfer(balances, src, dst, amount);
        unlock();
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

    pthread_t threads[2];
    int ret;

    int zero = 0;
    int one = 1;

    if ((ret = pthread_create(&threads[0], NULL, client_simulator, &zero)) != 0) {
        fprintf(stderr, "pthread_create(): %s\n", strerror(ret));
        return 1;
    }

    if ((ret = pthread_create(&threads[1], NULL, client_simulator, &one)) != 0) {
        fprintf(stderr, "pthread_create(): %s\n", strerror(ret));
        return 1;
    }

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    int final_total = total_balance();
    printf("Final total balance: %d\n", final_total);

    return 0;
}

