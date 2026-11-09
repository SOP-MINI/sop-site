/**
 * \brief Basic file memory mapping example.
 *
 * Process maps file into memory and fills it with digits.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, const char* argv[]) {

    if (argc != 2) {
      fprintf(stderr, "Filename required!\n");
      return 1;
    }

    srand(getpid());

    printf("My PID = %d\n", getpid());

    int fd = open(argv[1], O_RDWR); // TODO: Try to modify access mode here
    if (fd < 0) {
        perror("open()");
        return 1;
    }

    struct stat s;
    if (fstat(fd, &s) < 0) {
        perror("fstat()");
        return 1;
    }

    printf("> Map?");
    getchar();

    void *ptr = mmap(NULL, s.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // TODO: Try to remove PROT_WRITE, try to set MAP_PRIVATE
    if (ptr == MAP_FAILED) {
        perror("mmap()");
        return 1;
    }

    // TODO: Observe contents of /proc/[pid]/maps file

    close(fd);

    printf("Mapped addres = %p\n", ptr);

    printf("> Fill the file?");
    getchar();

    // TODO: Observe file contents with 'watch -n0.1 cat file.txt'
    for (int i = 0; i < 10; ++i) {
        char c = (char) ('0' + (rand() % 10));
        printf("Filling mmaped file with '%c' character\n", c);
        memset(ptr, c, s.st_size);
        sleep(1);
    }

    printf("> Unmap?");
    getchar();

    printf("Unmapping file\n");
    munmap(ptr, s.st_size);

    printf("> Exit?");
    getchar();

    return 0;
}
