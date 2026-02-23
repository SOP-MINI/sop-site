#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

// Global variable (BSS/Data segment)
int global_variable = 42;

void dummy_function() {
    // Just a placeholder to get a pointer to the text segment
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <filepath>\n", argv[0]);
        return 1;
    }
    const char* filepath = argv[1];

    // Local variable (Stack segment)
    int local_variable = 10;

    // 1. Standard Heap Allocation
    void* heap_ptr = malloc(1024);
    if (!heap_ptr) { perror("malloc"); return 1; }

    // 2. File Mapping
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) { perror("open"); return 1; }

    struct stat sb;
    fstat(fd, &sb);

    void* file_map_ptr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_map_ptr == MAP_FAILED) { perror("mmap file"); return 1; }
    close(fd);

    // 3. Anonymous Mapping (No file backing)
    size_t anon_size = 4096 * 10; // 10 pages
    void* anon_map_ptr = mmap(NULL, anon_size, PROT_READ | PROT_WRITE,
                              MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (anon_map_ptr == MAP_FAILED) { perror("mmap anon"); return 1; }

    // Print all addresses to cross-reference with /proc/<pid>/maps
    printf("Process PID: %d\n\n", getpid());

    printf("--- Memory Addresses in C ---\n");
    printf("[Text]  dummy_function:  %p\n", (void*)dummy_function);
    printf("[Data]  global_variable: %p\n", (void*)&global_variable);
    printf("[Heap]  malloc(1024):    %p\n", heap_ptr);
    printf("[Stack] local_variable:  %p\n", (void*)&local_variable);
    printf("[mmap]  file mapping:    %p\n", file_map_ptr);
    printf("[mmap]  anon mapping:    %p\n\n", anon_map_ptr);

    printf("Check another terminal and run:\n");
    printf("cat /proc/%d/maps\n\n", getpid());

    printf("Press ENTER to exit...\n");
    getchar();

    // Cleanup
    munmap(file_map_ptr, sb.st_size);
    munmap(anon_map_ptr, anon_size);
    free(heap_ptr);

    return 0;
}