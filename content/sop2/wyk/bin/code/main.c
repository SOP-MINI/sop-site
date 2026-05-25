#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int g_external;

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

    return f_defined();
}
