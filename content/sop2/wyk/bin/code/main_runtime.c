#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

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

    void *dl_handle = dlopen("libutils_shared.so", RTLD_LAZY);

    if (argc > 1 && strcmp(argv[1], "block") == 0)
        getchar();

    int (*utils_scale)(int) = dlsym(dl_handle, "utils_scale");

    if (argc > 1 && strcmp(argv[1], "block") == 0)
        getchar();

    return utils_scale(f_defined());
}
