#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    long page_size = sysconf(_SC_PAGESIZE);
    size_t num_pages = 64; // Small number to visualize easily
    size_t size = num_pages * page_size;

    char *memory = mmap(NULL, size, PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (memory == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    printf("PID: %d. Mapped %zu pages. Faulting them in...\n", getpid(), num_pages);
    for (size_t i = 0; i < num_pages; i++) {
        memory[i * page_size] = 1;
    }

    unsigned char *vec = malloc(num_pages);

    printf("Now monitoring mincore(). Open another terminal and allocate memory to force eviction.\n");
    printf("(e.g., using large_mmap or stress-ng)\n\n");
    while (1) {
        if (mincore(memory, size, vec) == -1) {
            perror("mincore");
            break;
        }

        printf("\r[");
        for (size_t i = 0; i < num_pages; i++) {
            if (vec[i] & 1) {
                printf("#"); // In RAM
            } else {
                printf("."); // Swapped/Evicted
            }
        }
        printf("]");
        fflush(stdout);
        usleep(500000); // 500ms
    }

    free(vec);
    return 0;
}
