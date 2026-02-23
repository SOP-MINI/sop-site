#define _GNU_SOURCE // for memmem()
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    // 1. Initial file preparation
    int fd = open(filepath, O_RDWR);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    // 2. Get file length
    struct stat sb;
    fstat(fd, &sb);
    size_t length = sb.st_size;

    // 3. Map the file into memory
    // TODO: Experiment with flags
    char* data = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }

    // fd no longer needed
    close(fd);

    printf("mmap() returned: %p\n", data);

    printf("Binary contents:\n");

    for (size_t i = 0; i < length; i++)
    {
        printf("0x%02x ", (unsigned char)data[i]);
        if (i % 8 == 7)
            printf("\n");
    }

    printf("\nPrinted as text:\n");

    for (size_t i = 0; i < length; i++)
    {
        char c = data[i];
        if (!isprint(c))
            c = '?';
        if (isspace(c))
            c = ' ';
        printf("'%c' ", c);
        if (i % 8 == 7)
            printf("\n");
    }

    printf("\n");

    // TODO: Safe?
    // printf("%s\n", data);

    // TODO: why not strstr()?
    char* ptr = memmem(data, length, "memories", strlen("memories"));
    if (ptr != NULL)
    {
        printf("Found 'memories' - replacing\n");
        memcpy(ptr, "_doubts_", strlen("_doubts_"));
    }

    // TODO: Discover where it ends
    // for (size_t i = 0;; i++)
        // printf("%020lu, 0x%02x\n", i, data[i]);

    // TODO: Can I do it?
    // data[length + 1] = 'X';

    // 5. Cleanup
    munmap(data, length);

    return 0;
}