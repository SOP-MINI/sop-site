#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <MB>\n", argv[0]);
        printf("Allocate memory larger than available RAM/cgroup to observe thrashing.\n");
        return 1;
    }

    size_t size = atoll(argv[1]) * 1024 * 1024;
    long page_size = sysconf(_SC_PAGESIZE);
    size_t num_pages = size / page_size;

    volatile char *memory = mmap(NULL, size, PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (memory == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    printf("Allocated %zu MB. Faulting in memory...\n", size / (1024*1024));
    
    // Initial faulting
    for (size_t i = 0; i < num_pages; i++) {
        memory[i * page_size] = 1;
    }
    
    printf("Starting random access...\n");
    srand(time(NULL));
    long long accesses = 0;
    time_t start = time(NULL);

    while (1) {
        for (int i = 0; i < 1000000; i++) {
            size_t random_page = rand() % num_pages;
            memory[random_page * page_size] = (char)random_page;
            accesses++;
        }
        
        time_t now = time(NULL);
        if (now - start >= 1) {
            printf("Accesses per second: %lld\n", accesses / (now - start));
            accesses = 0;
            start = now;
        }
    }

    return 0;
}
