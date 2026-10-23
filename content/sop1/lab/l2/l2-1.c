#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

#define FILE_BUF_LEN 256

void usage(const char *const pname)
{
    fprintf(stderr, "USAGE:%s path_1 path_2\n", pname);
    exit(EXIT_FAILURE);
}

int main(const int argc, const char *const *const argv)
{
    if (argc != 3)
        usage(argv[0]);

    const char *const path_1 = argv[1];
    const char *const path_2 = argv[2];

    const int fd_1 = open(path_1, O_RDONLY);
    if (fd_1 == -1)
        ERR("open");

    const int fd_2 = open(path_2, O_WRONLY | O_CREAT, 0777);
    if (fd_2 == -1)
        ERR("open");

    char file_buf[FILE_BUF_LEN];
    for (;;)
    {
        const ssize_t read_size = read(fd_1, file_buf, FILE_BUF_LEN);
        if (read_size == -1)
            ERR("bulk_read");

        if (read_size == 0)
            break;

        if (write(fd_2, file_buf, read_size) == -1)
            ERR("bulk_write");
    }

    if (close(fd_2) == -1)
        ERR("close");

    if (close(fd_1) == -1)
        ERR("close");

    return EXIT_SUCCESS;
}
