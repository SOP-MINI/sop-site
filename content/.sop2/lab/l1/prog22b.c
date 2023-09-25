#define _GNU_SOURCE
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ERR(source)                                                                                                    \
	(fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), perror(source), kill(0, SIGKILL), exit(EXIT_FAILURE))

// MAX_BUFF must be in one byte range
#define MAX_BUFF 200

volatile sig_atomic_t last_signal = 0;

int sethandler(void (*f)(int), int sigNo)
{
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	act.sa_handler = f;
	if (-1 == sigaction(sigNo, &act, NULL))
		return -1;
	return 0;
}

void sig_handler(int sig)
{
	last_signal = sig;
}

void sig_killme(int sig)
{
	if (rand() % 5 == 0)
		exit(EXIT_SUCCESS);
}

void sigchld_handler(int sig)
{
	pid_t pid;
	for (;;) {
		pid = waitpid(0, NULL, WNOHANG);
		if (0 == pid)
			return;
		if (0 >= pid) {
			if (ECHILD == errno)
				return;
			ERR("waitpid:");
		}
	}
}

void child_work(int fd, int R)
{
	char c, buf[MAX_BUFF + 1];
	unsigned char s;
	srand(getpid());
	if (sethandler(sig_killme, SIGINT))
		ERR("Setting SIGINT handler in child");
	for (;;) {
		if (TEMP_FAILURE_RETRY(read(fd, &c, 1)) < 1)
			ERR("read");
		s = 1 + rand() % MAX_BUFF;
		buf[0] = s;
		memset(buf + 1, c, s);
		if (TEMP_FAILURE_RETRY(write(R, buf, s + 1)) < 0)
			ERR("write to R");
	}
}

void parent_work(int n, int *fds, int R)
{
	unsigned char c;
	char buf[MAX_BUFF];
	int status, i;
	srand(getpid());
	if (sethandler(sig_handler, SIGINT))
		ERR("Setting SIGINT handler in parent");
	for (;;) {
		if (SIGINT == last_signal) {
			i = rand() % n;
			while (0 == fds[i % n] && i < 2 * n)
				i++;
			i %= n;
			if (fds[i]) {
				c = 'a' + rand() % ('z' - 'a');
				status = TEMP_FAILURE_RETRY(write(fds[i], &c, 1));
				if (status != 1) {
					if (TEMP_FAILURE_RETRY(close(fds[i])))
						ERR("close");
					fds[i] = 0;
				}
			}
			last_signal = 0;
		}
		status = read(R, &c, 1);
		if (status < 0 && errno == EINTR)
			continue;
		if (status < 0)
			ERR("read header from R");
		if (0 == status)
			break;
		if (TEMP_FAILURE_RETRY(read(R, buf, c)) < c)
			ERR("read data from R");
		buf[(int)c] = 0;
		printf("\n%s\n", buf);
	}
}

void create_children_and_pipes(int n, int *fds, int R)
{
	int tmpfd[2];
	int max = n;
	while (n) {
		if (pipe(tmpfd))
			ERR("pipe");
		switch (fork()) {
		case 0:
			while (n < max)
				if (fds[n] && TEMP_FAILURE_RETRY(close(fds[n++])))
					ERR("close");
			free(fds);
			if (TEMP_FAILURE_RETRY(close(tmpfd[1])))
				ERR("close");
			child_work(tmpfd[0], R);
			if (TEMP_FAILURE_RETRY(close(tmpfd[0])))
				ERR("close");
			if (TEMP_FAILURE_RETRY(close(R)))
				ERR("close");
			exit(EXIT_SUCCESS);

		case -1:
			ERR("Fork:");
		}
		if (TEMP_FAILURE_RETRY(close(tmpfd[0])))
			ERR("close");
		fds[--n] = tmpfd[1];
	}
}

void usage(char *name)
{
	fprintf(stderr, "USAGE: %s n\n", name);
	fprintf(stderr, "0<n<=10 - number of children\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	int n, *fds, R[2];
	if (2 != argc)
		usage(argv[0]);
	n = atoi(argv[1]);
	if (n <= 0 || n > 10)
		usage(argv[0]);
	if (sethandler(SIG_IGN, SIGINT))
		ERR("Setting SIGINT handler");
	if (sethandler(SIG_IGN, SIGPIPE))
		ERR("Setting SIGINT handler");
	if (sethandler(sigchld_handler, SIGCHLD))
		ERR("Setting parent SIGCHLD:");
	if (pipe(R))
		ERR("pipe");
	if (NULL == (fds = (int *)malloc(sizeof(int) * n)))
		ERR("malloc");
	create_children_and_pipes(n, fds, R[1]);
	if (TEMP_FAILURE_RETRY(close(R[1])))
		ERR("close");
	parent_work(n, fds, R[0]);
	while (n--)
		if (fds[n] && TEMP_FAILURE_RETRY(close(fds[n])))
			ERR("close");
	if (TEMP_FAILURE_RETRY(close(R[0])))
		ERR("close");
	free(fds);
	return EXIT_SUCCESS;
}
