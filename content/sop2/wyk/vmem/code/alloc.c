#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define GB (1024ULL * 1024ULL * 1024ULL)

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("%s <GB to alloc>\n", argv[0]);
        return 1;
    }

    long long gb = atoll(argv[1]);
    size_t size = (size_t)gb * GB;

    printf("Allocating %lld GB (VIRT)...\n", gb);

    volatile char *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (ptr == MAP_FAILED) {
        perror("mmap() failed");
        return 1;
    }

    printf("mmap() returned %p\n", ptr);

    for (size_t i = 0; i < size; ++i)
    {
        if (i % GB == 0)
            printf("filled %lld GB...\n", i / GB);
        ptr[i] = 0xAA;
    }


    printf("filling done!\n");

    munmap((void*)ptr, size);
    return 0;
}