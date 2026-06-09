#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

#define ALLOC_GIGABYTES 10ULL
#define ALLOC_SIZE (ALLOC_GIGABYTES * 1024 * 1024 * 1024)

long long get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

int main() {
    long page_size = sysconf(_SC_PAGESIZE);
    unsigned long num_pages = ALLOC_SIZE / page_size;

    volatile char *memory = mmap(NULL, ALLOC_SIZE, PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (memory == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }
    // Disable hugepage-backed mapping  // TODO: enable it!
    madvise((void*)memory, ALLOC_SIZE, MADV_NOHUGEPAGE);

    long long start_time = get_time_ns();

    for (unsigned long i = 0; i < num_pages; i++) {
        memory[i * page_size] = 1;
    }

    long long end_time = get_time_ns();

    double pass1_ms = (end_time - start_time) / 1000000.0;
    printf("1st pass: %8.2f ms\n", pass1_ms);

    start_time = get_time_ns();

    for (unsigned long i = 0; i < num_pages; i++) {
        memory[i * page_size] = 2;
    }

    end_time = get_time_ns();

    double pass2_ms = (end_time - start_time) / 1000000.0;

    printf("2nd pass: %8.2f ms\n", pass2_ms);

    munmap((void*)memory, ALLOC_SIZE);
    return 0;
}