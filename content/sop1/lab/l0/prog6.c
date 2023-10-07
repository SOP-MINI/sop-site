#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage(char *pname)
{
    fprintf(stderr, "USAGE:%s ([-t x] -n Name ... )\n", pname);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    int c, i;
    int x = 1;
    while ((c = getopt(argc, argv, "t:n:")) != -1)
        switch (c)
        {
            case 't':
                x = atoi(optarg);
                break;
            case 'n':
                for (i = 0; i < x; i++)
                    printf("Hello %s\n", optarg);
                break;
            case '?':
            default:
                usage(argv[0]);
        }
    if (argc > optind)
        usage(argv[0]);
    return EXIT_SUCCESS;
}
