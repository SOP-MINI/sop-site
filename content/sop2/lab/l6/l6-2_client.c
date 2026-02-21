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
    fprintf(stderr, "USAGE: %s server_pid\n", name);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
        usage(argv[0]);

    const int server_pid = atoi(argv[1]);
    if (server_pid == 0)
        usage(argv[0]);

    srand(getpid());

    int shm_fd;
    char shm_name[32];
    sprintf(shm_name, "/%d-board", server_pid);

    if ((shm_fd = shm_open(shm_name, O_RDWR, 0666)) == -1)
        ERR("shm_open");

    char* shm_ptr;
    if ((shm_ptr = (char*)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
        ERR("mmap");

    pthread_mutex_t* mutex = (pthread_mutex_t*)shm_ptr;
    char* N_shared = (shm_ptr + sizeof(pthread_mutex_t));
    char* board = (shm_ptr + sizeof(pthread_mutex_t)) + 1;
    const int N = N_shared[0];

    int score = 0;
    while (1)
    {
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

        const int D = 1 + rand() % 9;
        if (D == 1)
        {
            printf("Ops...\n");
            exit(EXIT_SUCCESS);
        }

        int x = rand() % N, y = rand() % N;
        printf("trying to search field (%d,%d)\n", x, y);
        const int p = board[N * y + x];
        if (p == 0)
        {
            printf("GAME OVER: score %d\n", score);
            pthread_mutex_unlock(mutex);
            break;
        }
        else
        {
            printf("found %d points\n", p);
            score += p;
            board[N * y + x] = 0;
        }

        pthread_mutex_unlock(mutex);
        struct timespec t = {1, 0};
        nanosleep(&t, &t);
    }

    munmap(shm_ptr, SHM_SIZE);

    return EXIT_SUCCESS;
}
