/* Problems with Non-Atomic Access - an example from
 * "The GNU C Library" documentation
 * The program fills mem struct alternatively with 0,0 and 1,1.
 * However, on most machines it takes several instructions to store
 * a new pair of integers in mem. If the signal is delivered in between these
 * instruction the handler might find that mem.a is 0 and mem.b is 1
 * (or vice versa).
 */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

/* define _LONG_ for x64 architecture */
#define _LONG_

#ifdef _LONG_
struct two_words
{
    long a, b;
} mem;
#else
struct two_words
{
    int a, b;
} mem;
#endif

void handler(int signum)
{
#ifdef _LONG_
    printf("%ld,%ld\n", mem.a, mem.b);
#else
    printf("%d,%d\n", mem.a, mem.b);
#endif
    alarm(1);
}

int main(void)
{
    static struct two_words zeros = {0, 0}, ones = {1, 1};
    signal(SIGALRM, handler);
    mem = zeros;
    alarm(1);
    while (1)
    {
        mem = zeros;
        mem = ones;
    }
    return 0;
}
