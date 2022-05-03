#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

int main(int argc, char **argv)
{
	char name[22];
	scanf("%21s", name);
	if (strlen(name) > 20)
		ERR("Name too long");
	printf("Hello %s\n", name);
	return EXIT_SUCCESS;
}
