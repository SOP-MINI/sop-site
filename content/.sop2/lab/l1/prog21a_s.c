#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

void usage(char *name)
{
    fprintf(stderr, "USAGE: %s fifo_file\n", name);
    exit(EXIT_FAILURE);
}

void read_from_fifo(int fifo)
{
    ssize_t count;
    char c;
    do
    {
        if ((count = read(fifo, &c, 1)) < 0)
            ERR("read");
        if (count > 0 && isalnum(c))
            printf("%c", c);
    } while (count > 0);
}

int main(int argc, char **argv)
{
    int fifo;
    if (argc != 2)
        usage(argv[0]);

    if (mkfifo(argv[1], S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) < 0)
        if (errno != EEXIST)
            ERR("create fifo");
    if ((fifo = open(argv[1], O_RDONLY)) < 0)
        ERR("open");
    read_from_fifo(fifo);
    if (close(fifo) < 0)
        ERR("close fifo:");
    return EXIT_SUCCESS;
}
