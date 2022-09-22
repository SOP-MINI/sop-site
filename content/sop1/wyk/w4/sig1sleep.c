/* sig1sleep.c. Illustrotion of
 * - setting a signal handler with sigaction()
 * - a side-effect of signal handling - early termination of sleep() call
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void sethandler(int sig, void (*f)(int))
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = f;
	if (sigaction(sig, &sa, NULL) == -1) {
		perror("sigaction:");
		exit(1);
	}
}

void handler(int sig)
{
	printf("SIGINT handler\n");
	/*
     * Wait a moment, increasing a chance for the SIGINT signal to be delivered
     * before returning from this signal handler.
     */
	sleep(1);
}

int main(int argc, char *argv[])
{
	int i;

	/*
     *	Install SIGINT signal handler
     */
	sethandler(SIGINT, handler);

	printf("%s: PID=%lu\n", argv[0], (unsigned long)getpid());

	/*
     *	Call pause() 10 times, waiting each time for end of signal handling.
     */
	for (i = 0; i < 10; i++) {
		int t;
		printf("waiting for a signal\n");
		t = sleep(5);
		printf("got a signal, t=%d\n", t);
	}

	return 0;
}
