#define _XOPEN_SOURCE 500
#include <dirent.h>
#include <errno.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#define MAXFD 20

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

int dirs = 0, files = 0, links = 0, other = 0;

int walk(const char *name, const struct stat *s, int type, struct FTW *f)
{
    switch (type)
    {
        case FTW_DNR:
        case FTW_D:
            dirs++;
            break;
        case FTW_F:
            files++;
            break;
        case FTW_SL:
            links++;
            break;
        default:
            other++;
    }
    return 0;
}

int main(int argc, char **argv)
{
    int i;
    for (i = 1; i < argc; i++)
    {
        if (nftw(argv[i], walk, MAXFD, FTW_PHYS) == 0)
            printf("%s:\nfiles:%d\ndirs:%d\nlinks:%d\nother:%d\n", argv[i], files, dirs, links, other);
        else
            printf("%s: brak dostępu\n", argv[i]);
        dirs = files = links = other = 0;
    }
    return EXIT_SUCCESS;
}
