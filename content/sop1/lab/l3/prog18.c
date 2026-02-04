#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define BIN_COUNT 11
#define NEXT_DOUBLE(seedptr) ((double)rand_r(seedptr) / (double)RAND_MAX)
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

typedef struct shared_state
{
    int balls_waiting;
    int bins[BIN_COUNT];
    int balls_thrown;
    pthread_mutex_t bins_mtx[BIN_COUNT];
    pthread_mutex_t balls_waiting_mtx;
    pthread_mutex_t balls_thrown_mtx;
} shared_state_t;

typedef struct thrower_args
{
    pthread_t tid;
    unsigned int seed;
    shared_state_t* shared;
} thrower_args_t;

void make_throwers(thrower_args_t** args_ptr, int num_throwers, shared_state_t* shared);
void* throwing_func(void* args);
int throw_ball(unsigned int* seedptr);

void usage(char* pname)
{
    fprintf(stderr, "Usage: %s [num_balls>0] [num_throwers>0]", pname);
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
    int balls_count, throwers_count;
    if (argc != 3)
        usage(argv[0]);

    balls_count = atoi(argv[1]);
    throwers_count = atoi(argv[2]);

    if (balls_count <= 0 || throwers_count <= 0)
        usage(argv[0]);

    shared_state_t shared = {
        .balls_waiting = balls_count,
        .balls_thrown = 0,
        .balls_waiting_mtx = PTHREAD_MUTEX_INITIALIZER,
        .balls_thrown_mtx = PTHREAD_MUTEX_INITIALIZER,
        .bins = {0},
    };

    int balls_thrown = shared.balls_thrown;

    for (int i = 0; i < BIN_COUNT; i++)
    {
        if (pthread_mutex_init(&shared.bins_mtx[i], NULL))
            ERR("pthread_mutex_init");
    }

    thrower_args_t* args;
    make_throwers(&args, throwers_count, &shared);

    while (balls_thrown < balls_count)
    {
        sleep(1);
        pthread_mutex_lock(&shared.balls_thrown_mtx);
        balls_thrown = shared.balls_thrown;
        pthread_mutex_unlock(&shared.balls_thrown_mtx);
    }

    int final_balls_count = 0;
    double mean_val = 0.0;
    for (int i = 0; i < BIN_COUNT; i++)
    {
        final_balls_count += shared.bins[i];
        mean_val += shared.bins[i] * i;
    }
    mean_val = mean_val / final_balls_count;

    printf("Bins count:\n");
    for (int i = 0; i < BIN_COUNT; i++)
        printf("%d\t", shared.bins[i]);
    printf("\nTotal balls count : %d\nMean value: %f\n", final_balls_count, mean_val);
    // for (int i = 0; i < BIN_COUNT; i++) pthread_mutex_destroy(&mxBins[i]);
    // free(args);
    // The resources used by detached threads cannod be freed as we are not sure
    // if they are running yet.
    exit(EXIT_SUCCESS);
}

void make_throwers(thrower_args_t** args_ptr, int throwers_count, shared_state_t* shared)
{
    *args_ptr = malloc(sizeof(thrower_args_t) * throwers_count);
    thrower_args_t* args = *args_ptr;
    if (args == NULL)
        ERR("malloc");

    pthread_attr_t attr;

    if (pthread_attr_init(&attr))
        ERR("pthread_attr_init");
    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
        ERR("pthread_attr_setdetachstate");

    srand(time(NULL));
    for (int i = 0; i < throwers_count; i++)
    {
        args[i].seed = rand();
        args[i].shared = shared;
        if (pthread_create(&args[i].tid, &attr, throwing_func, &args[i]))
            ERR("pthread_create");
    }

    pthread_attr_destroy(&attr);
}

void* throwing_func(void* void_args)
{
    thrower_args_t* args = void_args;
    while (1)
    {
        pthread_mutex_lock(&args->shared->balls_waiting_mtx);
        if (args->shared->balls_waiting > 0)
        {
            args->shared->balls_waiting--;
            pthread_mutex_unlock(&args->shared->balls_waiting_mtx);
        }
        else
        {
            pthread_mutex_unlock(&args->shared->balls_waiting_mtx);
            break;
        }

        int binno = throw_ball(&args->seed);
        pthread_mutex_lock(&args->shared->bins_mtx[binno]);
        args->shared->bins[binno]++;
        pthread_mutex_unlock(&args->shared->bins_mtx[binno]);
        pthread_mutex_lock(&args->shared->balls_thrown_mtx);
        args->shared->balls_thrown++;
        pthread_mutex_unlock(&args->shared->balls_thrown_mtx);
    }
    return NULL;
}

/* returns # of bin where ball has landed */
int throw_ball(unsigned int* seedptr)
{
    int result = 0;
    for (int i = 0; i < BIN_COUNT - 1; i++)
        if (NEXT_DOUBLE(seedptr) > 0.5)
            result++;
    return result;
}
