/* Illustration of function use: pthread_create(), pthread_join().
   Program creates 2 working threads, one outputs 10000 times "a",
   the other 10000 times "b".
 */

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

void *process(void *arg)
{
    int i;
    fprintf(stderr, "process(%s) is starting\n", (char *)arg);
    for (i = 0; i < 10000; i++)
    {
        write(1, (char *)arg, 1);
    }
    return NULL;
}

int main(void)
{
    int retcode;
    pthread_t th_a, th_b;
    void *retval;

    retcode = pthread_create(&th_a, NULL, process, (void *)"a");
    if (retcode != 0)
        fprintf(stderr, "pthread_create error (a): %d\n", retcode);
    retcode = pthread_create(&th_b, NULL, process, (void *)"b");
    if (retcode != 0)
        fprintf(stderr, "pthread_create error (b): %d\n", retcode);
    retcode = pthread_join(th_a, &retval);
    if (retcode != 0)
        fprintf(stderr, "pthread_join error (a): %d\n", retcode);
    retcode = pthread_join(th_b, &retval);
    if (retcode != 0)
        fprintf(stderr, "pthread_join error (b): %d\n", retcode);
    return 0;
}
