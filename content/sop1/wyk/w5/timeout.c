/* timeout.c - the program is copying lines of text from the standard
 * input. If a line is not entered within 5 seconds - the program exits (time-out).
 * See what happens if the program is run with a parameter ...
 */
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#define	TIMEOUT	5
void hand(int sig)
{
	fprintf(stderr, "Timeout!\n");
	return;
}

int main(int argc, char *argv[])
{
	char buf[20];
	int n;
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = hand;
	if (argc > 1)
		sa.sa_flags = SA_RESTART;
	if (sigaction(SIGALRM, &sa, NULL)) {
		perror("sigaction");
		return 1;
	}
	alarm(TIMEOUT);		/* setting the timeout */
	fprintf(stderr, "Timeout time=%ds\n", TIMEOUT);
	while ((n = read(0, buf, sizeof(buf))) > 0) {
		if (write(1, buf, n) < 0)
			return 1;
		alarm(5);	/*re-setting the timeout */
	}
	if(errno) perror("tralala");
	return 0;
}
