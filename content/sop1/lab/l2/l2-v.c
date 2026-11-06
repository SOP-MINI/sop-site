#define _POSIX_C_SOURCE 200809L

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

typedef struct
{
    unsigned int size;
    char title[65];
} Book;

void usage(const char* pname)
{
    fprintf(stderr, "USAGE:%s database output\n", pname);
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
    if (argc != 3)
        usage(argv[0]);

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        ERR("open");
    struct stat s;
    if (fstat(fd, &s) < 0)
        ERR("fstat");
    const int size = s.st_size;
    const int N = size / 68;

    struct iovec* buffers = malloc(N * sizeof(struct iovec));
    Book* books = malloc(N * sizeof(Book));
    if (buffers == NULL || books == NULL)
        ERR("malloc");

    for (int i = 0; i < N; i++)
    {
        buffers[i].iov_base = &books[i];
        buffers[i].iov_len = 68;
        books[i].title[64] = 0;
    }
    if (readv(fd, buffers, N) < size)
        ERR("readv");
    if (close(fd))
        ERR("close");

    for (int i = 0; i < N; i++)
    {
        printf("Size: %u Title: %s\n", books[i].size, books[i].title);
        books[i].size++;
    }

    if ((fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
        ERR("open");
    if (writev(fd, buffers, N) < size)
        ERR("writev");
    if (close(fd))
        ERR("close");

    free(buffers);
    free(books);
    return EXIT_SUCCESS;
}
