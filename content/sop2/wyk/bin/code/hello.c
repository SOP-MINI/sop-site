#include <stdio.h>

const int g_constant = 1;
int g_initialized = 2;
int g_initialized_unused = 404;
int g_uninitialized;
extern int g_external;
// extern int g_undefined; // TODO: Try to use

int f_external();

static int f_static();

int f_defined()
{
    int acc = g_constant;
    acc += g_initialized;
    acc += g_uninitialized;
    acc += f_external();
    acc += f_static();
    return acc;
}

int f_static()
{
    return 200;
}


void f_print()
{
    printf("g_constant at %p\n", &g_constant);
    printf("g_initialized at %p\n", &g_initialized);
    printf("g_uninitialized at %p\n", &g_uninitialized);
    printf("g_external at %p\n", &g_external);

    printf("f_external() at %p\n", f_external);
    printf("f_static() at %p\n", f_static);
    printf("f_defined() at %p\n", f_defined);
    printf("f_print() at %p\n", f_print);
}
