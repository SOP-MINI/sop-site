#include <pthread.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096
#define DEFAULT_N 1000
#define DEFAULT_K 10
#define BIN_COUNT 11
#define NEXT_DOUBLE(seedptr) ((double)rand_r(seedptr) / (double)RAND_MAX)
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

typedef unsigned int UINT;
typedef struct argsThrower
{
    pthread_t tid;
    UINT seed;
    int *pBallsThrown;
    int *pBallsWaiting;
    int *bins;
    pthread_mutex_t *mxBins;
    pthread_mutex_t *pmxBallsThrown;
    pthread_mutex_t *pmxBallsWaiting;
} argsThrower_t;

void ReadArguments(int argc, char **argv, int *ballsCount, int *throwersCount);
void make_throwers(argsThrower_t *argsArray, int throwersCount);
void *throwing_func(void *args);
int throwBall(UINT *seedptr);

int main(int argc, char **argv)
{
    int ballsCount, throwersCount;
    ReadArguments(argc, argv, &ballsCount, &throwersCount);
    int ballsThrown = 0, bt = 0;
    int ballsWaiting = ballsCount;
    pthread_mutex_t mxBallsThrown = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mxBallsWaiting = PTHREAD_MUTEX_INITIALIZER;
    int bins[BIN_COUNT];
    pthread_mutex_t mxBins[BIN_COUNT];
    for (int i = 0; i < BIN_COUNT; i++)
    {
        bins[i] = 0;
        if (pthread_mutex_init(&mxBins[i], NULL))
            ERR("Couldn't initialize mutex!");
    }
    argsThrower_t *args = (argsThrower_t *)malloc(sizeof(argsThrower_t) * throwersCount);
    if (args == NULL)
        ERR("Malloc error for throwers arguments!");
    srand(time(NULL));
    for (int i = 0; i < throwersCount; i++)
    {
        args[i].seed = (UINT)rand();
        args[i].pBallsThrown = &ballsThrown;
        args[i].pBallsWaiting = &ballsWaiting;
        args[i].bins = bins;
        args[i].pmxBallsThrown = &mxBallsThrown;
        args[i].pmxBallsWaiting = &mxBallsWaiting;
        args[i].mxBins = mxBins;
    }
    make_throwers(args, throwersCount);
    while (bt < ballsCount)
    {
        sleep(1);
        pthread_mutex_lock(&mxBallsThrown);
        bt = ballsThrown;
        pthread_mutex_unlock(&mxBallsThrown);
    }
    int realBallsCount = 0;
    double meanValue = 0.0;
    for (int i = 0; i < BIN_COUNT; i++)
    {
        realBallsCount += bins[i];
        meanValue += bins[i] * i;
    }
    meanValue = meanValue / realBallsCount;
    printf("Bins count:\n");
    for (int i = 0; i < BIN_COUNT; i++)
        printf("%d\t", bins[i]);
    printf("\nTotal balls count : %d\nMean value: %f\n", realBallsCount, meanValue);
    // for (int i = 0; i < BIN_COUNT; i++) pthread_mutex_destroy(&mxBins[i]);
    // free(args);
    // The resources used by detached threads cannod be freed as we are not sure
    // if they are running yet.
    exit(EXIT_SUCCESS);
}

void ReadArguments(int argc, char **argv, int *ballsCount, int *throwersCount)
{
    *ballsCount = DEFAULT_N;
    *throwersCount = DEFAULT_K;
    if (argc >= 2)
    {
        *ballsCount = atoi(argv[1]);
        if (*ballsCount <= 0)
        {
            printf("Invalid value for 'balls count'");
            exit(EXIT_FAILURE);
        }
    }
    if (argc >= 3)
    {
        *throwersCount = atoi(argv[2]);
        if (*throwersCount <= 0)
        {
            printf("Invalid value for 'throwers count'");
            exit(EXIT_FAILURE);
        }
    }
}

void make_throwers(argsThrower_t *argsArray, int throwersCount)
{
    pthread_attr_t threadAttr;
    if (pthread_attr_init(&threadAttr))
        ERR("Couldn't create pthread_attr_t");
    if (pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED))
        ERR("Couldn't setdetachsatate on pthread_attr_t");
    for (int i = 0; i < throwersCount; i++)
    {
        if (pthread_create(&argsArray[i].tid, &threadAttr, throwing_func, &argsArray[i]))
            ERR("Couldn't create thread");
    }
    pthread_attr_destroy(&threadAttr);
}

void *throwing_func(void *voidArgs)
{
    argsThrower_t *args = voidArgs;
    while (1)
    {
        pthread_mutex_lock(args->pmxBallsWaiting);
        if (*args->pBallsWaiting > 0)
        {
            (*args->pBallsWaiting) -= 1;
            pthread_mutex_unlock(args->pmxBallsWaiting);
        }
        else
        {
            pthread_mutex_unlock(args->pmxBallsWaiting);
            break;
        }
        int binno = throwBall(&args->seed);
        pthread_mutex_lock(&args->mxBins[binno]);
        args->bins[binno] += 1;
        pthread_mutex_unlock(&args->mxBins[binno]);
        pthread_mutex_lock(args->pmxBallsThrown);
        (*args->pBallsThrown) += 1;
        pthread_mutex_unlock(args->pmxBallsThrown);
    }
    return NULL;
}

/* returns # of bin where ball has landed */
int throwBall(UINT *seedptr)
{
    int result = 0;
    for (int i = 0; i < BIN_COUNT - 1; i++)
        if (NEXT_DOUBLE(seedptr) > 0.5)
            result++;
    return result;
}
