#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define ERR(source) \
    (fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), perror(source), kill(0, SIGKILL), exit(EXIT_FAILURE))

#define SHM_SIZE 1024

void usage(char* name)
{
    fprintf(stderr, "USAGE: %s N\n", name);
    fprintf(stderr, "3 < N <= 30 - board size\n");
    exit(EXIT_FAILURE);
}

typedef struct
{
    int running;
    pthread_mutex_t mutex;
    sigset_t old_mask, new_mask;
} sighandling_args_t;

void* sighandling(void* args)
{
    sighandling_args_t* sighandling_args = (sighandling_args_t*)args;
    int signo;
    if (sigwait(&sighandling_args->new_mask, &signo))
        ERR("sigwait failed.");
    if (signo != SIGINT)
    {
        ERR("unexpected signal");
    }

    pthread_mutex_lock(&sighandling_args->mutex);
    sighandling_args->running = 0;
    pthread_mutex_unlock(&sighandling_args->mutex);
    return NULL;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
        usage(argv[0]);

    const int N = atoi(argv[1]);
    if (N < 3 || N >= 100)
        usage(argv[0]);

    const pid_t pid = getpid();
    srand(pid);

    printf("My PID is %d\n", pid);
    int shm_fd;
    char shm_name[32];
    sprintf(shm_name, "/%d-board", pid);

    if ((shm_fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0666)) == -1)
        ERR("shm_open");
    if (ftruncate(shm_fd, SHM_SIZE) == -1)
        ERR("ftruncate");

    char* shm_ptr;
    if ((shm_ptr = (char*)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
        ERR("mmap");

    pthread_mutex_t* mutex = (pthread_mutex_t*)shm_ptr;
    char* N_shared = (shm_ptr + sizeof(pthread_mutex_t));
    char* board = (shm_ptr + sizeof(pthread_mutex_t)) + 1;
    N_shared[0] = N;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            board[i * N + j] = 1 + rand() % 9;
        }
    }

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutexattr_setrobust(&mutex_attr, PTHREAD_MUTEX_ROBUST);
    pthread_mutex_init(mutex, &mutex_attr);

    sighandling_args_t sighandling_args = {1, PTHREAD_MUTEX_INITIALIZER};

    sigemptyset(&sighandling_args.new_mask);
    sigaddset(&sighandling_args.new_mask, SIGINT);
    if (pthread_sigmask(SIG_BLOCK, &sighandling_args.new_mask, &sighandling_args.old_mask))
        ERR("SIG_BLOCK error");

    pthread_t sighandling_thread;
    pthread_create(&sighandling_thread, NULL, sighandling, &sighandling_args);

    while (1)
    {
        pthread_mutex_lock(&sighandling_args.mutex);
        if (!sighandling_args.running)
            break;

        pthread_mutex_unlock(&sighandling_args.mutex);

        int error;
        if ((error = pthread_mutex_lock(mutex)) != 0)
        {
            if (error == EOWNERDEAD)
            {
                pthread_mutex_consistent(mutex);
            }
            else
            {
                ERR("pthread_mutex_lock");
            }
        }

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("%d", board[i * N + j]);
            }
            putchar('\n');
        }
        putchar('\n');
        pthread_mutex_unlock(mutex);
        struct timespec t = {3, 0};
        nanosleep(&t, &t);
    }

    pthread_join(sighandling_thread, NULL);

    pthread_mutexattr_destroy(&mutex_attr);
    pthread_mutex_destroy(mutex);

    munmap(shm_ptr, SHM_SIZE);
    shm_unlink(shm_name);

    return EXIT_SUCCESS;
}
