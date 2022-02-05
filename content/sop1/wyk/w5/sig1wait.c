 /* sig1wait.c - a simple illustration of sigwait() use for "synchroneous
  * receiving of asynchroneous signals".
  */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

sigset_t signalSet1, signalSet2;

int main(void)
{
	int n=5, sigNo;
	sigfillset(&signalSet1);
	sigemptyset(&signalSet2);
	/*
	 * Block signals.
	 */
	sigprocmask(SIG_BLOCK, &signalSet1, NULL);
	sigaddset(&signalSet2,SIGINT);
	while (n-->0) {
		int t;
		if (sigwait(&signalSet2, &sigNo)) {
			perror("sigwait");;
			break;
		}
		t=sleep(5);
		fprintf(stderr, "t=%d, sigNo=%d\n", t, sigNo);
	}
	printf("Bye!\n");
	return 0;
}
