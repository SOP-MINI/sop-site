#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 20

int main(int argc, char **argv)
{
    int x, i;
    char *env = getenv("TIMES");
    if (env)
        x = atoi(env);
    else
        x = 1;
    char name[MAX_LINE + 2];
    while (fgets(name, MAX_LINE + 2, stdin) != NULL)
        for (i = 0; i < x; i++)
            printf("Hello %s", name);
    if (putenv("RESULT=Done") != 0)
    {
        fprintf(stderr, "putenv failed");
        return EXIT_FAILURE;
    }
    printf("%s\n", getenv("RESULT"));
    if (system("env|grep RESULT") != 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
