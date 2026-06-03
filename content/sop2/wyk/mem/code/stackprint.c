#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    printf("pid = %d\n", getpid());
    FILE* fa = fopen("stackprint.addr", "w");
    FILE* fp = fopen("stackprint.pid", "w");
    int x = 0;
    fprintf(fa, "%p", &x);
    fprintf(fp, "%d", getpid());
    fclose(fa);
    fclose(fp);
    for (int i = 0;; ++i)
    {
        x = i;
        printf("&x = %p\n", &x);
        sleep(1);
    }
    return 0;
}