#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int g_external;

// libutils.so
extern int utils_magic;
int utils_scale(int v);

int f_external()
{
    return 200;
}

int f_defined();

void f_print();

int main(int argc, char** argv)
{
    f_print();

    printf("main() at %p\n", main);

    if (argc > 1 && strcmp(argv[1], "block") == 0)
        getchar();

    return utils_scale(f_defined());
}
