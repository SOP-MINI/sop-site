#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

#define PLAYER_COUNT 4
#define ROUNDS 10

struct arguments
{
    int id;
    unsigned int seed;
    int* scores;
    int* rolls;
    pthread_barrier_t *barrier;
};

void* thread_func(void *arg) {
    struct arguments *args = (struct arguments *)arg;
    for (int round = 0; round < ROUNDS; ++round) {
        args->rolls[args->id] = 1 + rand_r(&args->seed) % 6;
        printf("player %d: Rolled %d.\n", args->id, args->rolls[args->id]);
        
        int result = pthread_barrier_wait(args->barrier);
        
        if(result == PTHREAD_BARRIER_SERIAL_THREAD) {
            printf("player %d: Assigning scores.\n", args->id);
            int max = -1;
            for (int i = 0; i < PLAYER_COUNT; ++i) {
                int roll = args->rolls[i];
                if(roll > max) {
                    max = roll;
                }
            }
            for (int i = 0; i < PLAYER_COUNT; ++i) {
                int roll = args->rolls[i];
                if(roll == max) {
                    args->scores[i] = args->scores[i] + 1;
                    printf("player %d: Player %d got a point.\n", args->id, i);
                }
            }
        }
        pthread_barrier_wait(args->barrier);
    }
    
    return NULL;
}

void create_threads(pthread_t *thread, struct arguments *targ, pthread_barrier_t *barrier, int *scores, int* rolls)
{
    srand(time(NULL));
    int i;
    for (i = 0; i < PLAYER_COUNT; i++)
    {
        targ[i].id = i;
        targ[i].seed = rand();
        targ[i].scores = scores;
        targ[i].rolls = rolls;
        targ[i].barrier = barrier;
        if (pthread_create(&thread[i], NULL, thread_func, (void *)&targ[i]) != 0)
            ERR("pthread_create");
    }
}

int main() {
    pthread_t threads[PLAYER_COUNT];
    struct arguments targ[PLAYER_COUNT];
    int scores[PLAYER_COUNT] = {0};
    int rolls[PLAYER_COUNT];
    pthread_barrier_t barrier;
    
    pthread_barrier_init(&barrier, NULL, PLAYER_COUNT);
    
    create_threads(threads, targ, &barrier, scores, rolls);
    
    for (int i = 0; i < PLAYER_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    
    puts("Scores: ");
    for (int i = 0; i < PLAYER_COUNT; ++i) {
        printf("ID %d: %i\n", i, scores[i]);
    }
    
    pthread_barrier_destroy(&barrier);
    
    return 0;
}
