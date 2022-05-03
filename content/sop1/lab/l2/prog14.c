#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ERR(source)                                                                                                    \
	(fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), perror(source), kill(0, SIGKILL), exit(EXIT_FAILURE))

volatile sig_atomic_t last_signal = 0;

void sethandler(void (*f)(int), int sigNo)
{
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	act.sa_handler = f;
	if (-1 == sigaction(sigNo, &act, NULL))
		ERR("sigaction");
}

void sig_handler(int sig)
{
	printf("[%d] received signal %d\n", getpid(), sig);
	last_signal = sig;
}

void sigchld_handler(int sig)
{
	pid_t pid;
	for (;;) {
		pid = waitpid(0, NULL, WNOHANG);
		if (pid == 0)
			return;
		if (pid <= 0) {
			if (errno == ECHILD)
				return;
			ERR("waitpid");
		}
	}
}

void child_work(int l)
{
	int t, tt;
	srand(getpid());
	t = rand() % 6 + 5;
	while (l-- > 0) {
		for (tt = t; tt > 0; tt = sleep(tt))
			;
		if (last_signal == SIGUSR1)
			printf("Success [%d]\n", getpid());
		else
			printf("Failed [%d]\n", getpid());
	}
	printf("[%d] Terminates \n", getpid());
}

void parent_work(int k, int p, int l)
{
	struct timespec tk = { k, 0 };
	struct timespec tp = { p, 0 };
	sethandler(sig_handler, SIGALRM);
	alarm(l * 10);
	while (last_signal != SIGALRM) {
		nanosleep(&tk, NULL);
		if (kill(0, SIGUSR1) < 0)
			ERR("kill");
		nanosleep(&tp, NULL);
		if (kill(0, SIGUSR2) < 0)
			ERR("kill");
	}
	printf("[PARENT] Terminates \n");
}

void create_children(int n, int l)
{
	while (n-- > 0) {
		switch (fork()) {
		case 0:
			sethandler(sig_handler, SIGUSR1);
			sethandler(sig_handler, SIGUSR2);
			child_work(l);
			exit(EXIT_SUCCESS);
		case -1:
			perror("Fork:");
			exit(EXIT_FAILURE);
		}
	}
}

void usage(void)
{
	fprintf(stderr, "USAGE: signals n k p l\n");
	fprintf(stderr, "n - number of children\n");
	fprintf(stderr, "k - Interval before SIGUSR1\n");
	fprintf(stderr, "p - Interval before SIGUSR2\n");
	fprintf(stderr, "l - lifetime of child in cycles\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	int n, k, p, l;
	if (argc != 5)
		usage();
	n = atoi(argv[1]);
	k = atoi(argv[2]);
	p = atoi(argv[3]);
	l = atoi(argv[4]);
	if (n <= 0 || k <= 0 || p <= 0 || l <= 0)
		usage();
	sethandler(sigchld_handler, SIGCHLD);
	sethandler(SIG_IGN, SIGUSR1);
	sethandler(SIG_IGN, SIGUSR2);
	create_children(n, l);
	parent_work(k, p, l);
	while (wait(NULL) > 0)
		;
	return EXIT_SUCCESS;
}
