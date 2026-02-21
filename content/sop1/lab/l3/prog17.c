#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

typedef struct targs
{
    pthread_t tid;
    unsigned int seed;
    int samples_count;
} targs_t;

void* pi_estimation(void* args);

void usage(char* pname)
{
    fprintf(stderr, "Usage: %s [num_threads > 0] [num_samples > 0]\n", pname);
    exit(EXIT_FAILURE);
}

void create_threads(int thread_count, int samples_count, targs_t** estimations)
{
    *estimations = malloc(sizeof(targs_t) * thread_count);
    if (estimations == NULL)
        ERR("malloc");

    targs_t* targs = *estimations;

    srand(time(NULL));
    for (int i = 0; i < thread_count; i++)
    {
        targs[i].seed = rand();
        targs[i].samples_count = samples_count;

        if (pthread_create(&(targs[i].tid), NULL, pi_estimation, &targs[i]) != 0)
            ERR("pthread_create");
    }
}

int main(int argc, char** argv)
{
    if (argc != 3)
        usage(argv[0]);

    int thread_count = atoi(argv[1]);
    int samples_count = atoi(argv[2]);

    if (thread_count <= 0 || samples_count <= 0)
        usage(argv[0]);

    targs_t* estimations;
    create_threads(thread_count, samples_count, &estimations);

    int* subresult = NULL;
    int cumulative_result = 0;
    for (int i = 0; i < thread_count; i++)
    {
        if (pthread_join(estimations[i].tid, (void**)&subresult) != 0)
            ERR("pthread_join");

        if (NULL != subresult)
        {
            cumulative_result += *subresult;
            free(subresult);
        }
    }

    const double sum_avg = (double)cumulative_result / (double)thread_count;
    const double pi_estimate = (2.0 * (double)samples_count) / (sum_avg * sum_avg);
    printf("Estimated value of PI is %f\n", pi_estimate);

    free(estimations);
}

void* pi_estimation(void* void_ptr)
{
    targs_t* args = void_ptr;
    int* result = malloc(sizeof(int));
    if (result == NULL)
        ERR("malloc");

    int inner_count = 0;
    for (int i = 0; i < args->samples_count; i++)
    {
        inner_count += (rand_r(&args->seed) % 2) ? 1 : -1;
    }

    *result = abs(inner_count);

    return result;
}
