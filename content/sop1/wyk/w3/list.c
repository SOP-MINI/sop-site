/* list.c - a simple program, that lists files in each directory, specified
 * as program parameters. An illustration of opendir(), readdir(), getopt() use.
 * Suggested directions of improvements.
 * - add protections againts overflow of fname buffer.
 * - change values of DIRNAME_LEN and FNAME_LEN , so that they are
 *   related to system limits on directory and file sizes
 * - display content of the whole subdirectory recursively
 */

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DIRNAME_LEN 512
#define FNAME_LEN 1024

void usage()
{
    fprintf(stderr, "\nUsage:\n\nl [-il] [dir1 ...]\noptions:\n");
    fprintf(stderr, "i - inode number\n");
    fprintf(stderr, "l - size\n\n");
    exit(1);
}

int main(int argc, char **argv)
{
    char dirname[DIRNAME_LEN], fname[FNAME_LEN];
    int c, i_opt = 0, l_opt = 0;
    DIR *dir;
    struct dirent *ent;
    /* getting options */
    while ((c = getopt(argc, argv, "il")) != -1)
    {
        switch (c)
        {
            case 'i':
                i_opt = 1;
                break;
            case 'l':
                l_opt = 1;
                break;
            default:
                usage();
        }
    }
    do
    {
        if (optind < argc)
            strncpy(dirname, argv[optind++], sizeof(dirname));
        else
            strcpy(dirname, ".");
        if ((dir = opendir(dirname)) == NULL)
        {
            fprintf(stderr, "\nNo access to directory: %s\n", dirname);
            continue;
        }
        else
        {
            printf("\nDirectory: %s\n", dirname);
            if (i_opt)
                printf("    i-node ");
            if (l_opt)
                printf("      size ");
            if (i_opt || l_opt)
                printf("name\n");
        }
        while ((ent = readdir(dir)) != NULL)
        {
            if (i_opt)
                printf("%10ld ", ent->d_ino);
            if (l_opt)
            {
                struct stat buf;
                /* WARNING: fname buffer overflow protection should be added below ! */
                strcpy(fname, dirname);
                strcat(fname, "/");
                strcat(fname, ent->d_name);
                if (stat(fname, &buf) < 0)
                {
                    fprintf(stderr, "errno=%d, fname=%s\n", errno, fname);
                    if (errno == ESPIPE || errno == ENOENT)
                    {
                        printf("             ");
                        errno = 0;
                    }
                    else
                    {
                        perror(fname);
                        exit(2);
                    }
                }
                else
                    printf("%10ld ", buf.st_size);
            }
            printf("%s\n", ent->d_name);
        }
    } while (optind < argc);
    return 0;
}
