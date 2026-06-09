#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#define ALLOC_GIGABYTES 10ULL
#define ALLOC_SIZE (ALLOC_GIGABYTES * 1024 * 1024 * 1024)

int main() {
    long page_size = sysconf(_SC_PAGESIZE);
    unsigned long max_pages = ALLOC_SIZE / page_size;

    printf("pid =  %d\n", getpid());
    printf("PAGE_SIZE = %ld bajtów\n", page_size);

    printf("Ready to mmap()? >");
    getchar();

    char *memory = mmap(NULL, ALLOC_SIZE, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // Disable huge page (2MB) backed mapping // TODO: enable it!
    madvise(memory, ALLOC_SIZE, MADV_NOHUGEPAGE);

    if (memory == MAP_FAILED) {
        perror("mmap() failed");
        return 1;
    }

    printf("mmap()'ed %llu GB (MAP_PRIVATE | MAP_ANONYMOUS) at %p\n", ALLOC_GIGABYTES, memory);

    unsigned long current_page = 0;

    while (current_page < max_pages) {
        long pages_to_touch;
        printf("[%ld/%ld]> How many pages to 'touch'? (256 pages = 1 MB, <=0 to exit): ", current_page, max_pages);
        if (scanf("%ld", &pages_to_touch) != 1 || pages_to_touch <= 0) break;

        if (current_page + pages_to_touch > max_pages) {
            pages_to_touch = max_pages - current_page;
        }

        while (pages_to_touch > 0) {
            for (long j = 0; j < page_size; ++j)
            {
                memory[current_page * page_size + page_size] = 0xAA;
            }
            pages_to_touch--;
            current_page++;
        }

        printf("Done.\n");
    }

    munmap(memory, ALLOC_SIZE);
    return 0;
}