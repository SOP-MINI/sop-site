#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define SHM_NAME "/hello.shm"
#define SHM_SIZE 128

int main()
{
    // Create a POSIX Shared Memory object
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    // Set the size
    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    // Map it into our address space
    char* shared_mem = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_mem == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }
    close(fd); // fd is no longer needed after mmap

    // Write some data
    strcpy(shared_mem, "Hello, POSIX Shared Memory World!\n");

    // --- PAUSE FOR /dev/shm DEMONSTRATION ---
    printf("\n*** PAUSED ***\n");
    getchar();
    // -------------------------------

    for (int i = 0; i < 10; i++)
    {
        char c = rand() % ('z' - 'a') + 'a';
        printf("Filling SHM with %c...\n", c);
        memset(shared_mem, c, SHM_SIZE);
        sleep(1);
    }

    // --- PAUSE FOR /dev/shm DEMONSTRATION ---
    printf("\n*** PAUSED ***\n");
    getchar();
    // -------------------------------

    // Cleanup
    munmap(shared_mem, SHM_SIZE);

    // If we don't unlink, the object stays in RAM until reboot!
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
    }

    return 0;
}
