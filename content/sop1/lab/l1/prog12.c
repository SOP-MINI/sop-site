#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

void usage(char *pname)
{
	fprintf(stderr, "USAGE:%s -n Name -p OCTAL -s SIZE\n", pname);
	exit(EXIT_FAILURE);
}

void make_file(char *name, ssize_t size, mode_t perms, int percent)
{
	FILE *s1;
	int i;
	umask(~perms & 0777);
	if ((s1 = fopen(name, "w+")) == NULL)
		ERR("fopen");
	for (i = 0; i < (size * percent) / 100; i++) {
		if (fseek(s1, rand() % size, SEEK_SET))
			ERR("fseek");
		fprintf(s1, "%c", 'A' + (i % ('Z' - 'A' + 1)));
	}
	if (fclose(s1))
		ERR("fclose");
}

int main(int argc, char **argv)
{
	int c;
	char *name = NULL;
	mode_t perms = -1;
	ssize_t size = -1;
	while ((c = getopt(argc, argv, "p:n:s:")) != -1)
		switch (c) {
		case 'p':
			perms = strtol(optarg, (char **)NULL, 8);
			break;
		case 's':
			size = strtol(optarg, (char **)NULL, 10);
			break;
		case 'n':
			name = optarg;
			break;
		case '?':
		default:
			usage(argv[0]);
		}
	if ((NULL == name) || ((mode_t)-1 == perms) || (-1 == size))
		usage(argv[0]);
	if (unlink(name) && errno != ENOENT)
		ERR("unlink");
	srand(time(NULL));
	make_file(name, size, perms, 10);
	return EXIT_SUCCESS;
}
