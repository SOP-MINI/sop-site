/**
 * \brief Basic shared memory example.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define SHM_SIZE 32

int main() {

    srand(getpid());

    printf("My PID = %d\n", getpid());

    shm_unlink("/sop_shm");

    int fd = shm_open("/sop_shm", O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd < 0) {
        perror("shm_open()");
        return 1;
    }

    if (ftruncate(fd, SHM_SIZE) < 0) {
        perror("ftruncate()");
        return 1;
    }

    printf("> Map?");
    getchar();

    void *ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // TODO: Try to remove PROT_WRITE, try to set MAP_PRIVATE
    if (ptr == MAP_FAILED) {
        perror("mmap()");
        return 1;
    }

    // TODO: Observe contents of /proc/[pid]/maps file
    // TODO: Check /dev/shm contents

    close(fd);

    printf("Mapped addres = %p\n", ptr);

    printf("> Fill?");
    getchar();

    switch (fork()) {
        case -1:
            perror("fork()");
            return 1;
        case 0: {
            // child
            for (int i = 0; i < 10; ++i) {
                memset(ptr, (unsigned char) (rand() % 255), SHM_SIZE);
                sleep(1);
            }
            exit(0);
        }
        default: {
            // parent
            for (int i = 0; i < 10; ++i) {
                // TODO: Observe non-atomic updates of memory block
                // TODO: Observe `watch -n0.1 xxd /dev/shm/sop_shm`
                for (int j = 0; j < SHM_SIZE; j++)
                    printf("%02x ", ((unsigned char *) ptr)[j]);
                printf("\n");
                sleep(1);
            }
            while (wait(NULL) > 0);
            break;
        }
    }

    printf("> Unmap?");
    getchar();

    printf("Unmapping file\n");
    munmap(ptr, SHM_SIZE);

    printf("> Exit?");
    getchar();

    return 0;
}
