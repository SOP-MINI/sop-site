#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 20

int main(int argc, char **argv)
{
	char name[MAX_LINE + 2];
	while (fgets(name, MAX_LINE + 2, stdin) != NULL)
		printf("Hello %s", name);
	return EXIT_SUCCESS;
}
