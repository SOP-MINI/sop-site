#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <filepath>\n", argv[0]);
        return 1;
    }
    const char* filepath = argv[1];

    // 1. Open the file (create if necessary)
    int fd = open(filepath, O_RDWR | O_CREAT, 0666);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    // We only need space for a single integer
    ssize_t length = sizeof(int);

    struct stat sb;
    if (fstat(fd, &sb) < 0)
    {
        perror("fstat");
        return 1;
    }
    if (sb.st_size < length)
    {
        if (ftruncate(fd, length) == -1)
        {
            perror("ftruncate");
            close(fd);
            return 1;
        }
    }

    // 2. Map the integer into memory (MAP_SHARED is crucial for IPC)
    int* shared_counter = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_counter == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }
    close(fd);

    printf("Starting increment loop. Initial value: %d\n", *shared_counter);

    // 3. The danger zone: non-atomic increments
    const int iterations = 1000000000;
    for (int i = 0; i < iterations; i++)
    {
       ++*shared_counter;
    }

    printf("Finished! Final value: %d\n", *shared_counter);

    // 4. Cleanup
    munmap(shared_counter, length);

    return 0;
}
