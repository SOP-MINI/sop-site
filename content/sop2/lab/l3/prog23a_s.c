#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

#define BACKLOG 3

volatile sig_atomic_t do_work = 1;

void sigint_handler(int sig)
{
	do_work = 0;
}

int sethandler(void (*f)(int), int sigNo)
{
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	act.sa_handler = f;
	if (-1 == sigaction(sigNo, &act, NULL))
		return -1;
	return 0;
}

int make_socket(char *name, struct sockaddr_un *addr)
{
	int socketfd;
	if ((socketfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0)
		ERR("socket");
	memset(addr, 0, sizeof(struct sockaddr_un));
	addr->sun_family = AF_UNIX;
	strncpy(addr->sun_path, name, sizeof(addr->sun_path) - 1);
	return socketfd;
}

int bind_socket(char *name)
{
	struct sockaddr_un addr;
	int socketfd;
	if (unlink(name) < 0 && errno != ENOENT)
		ERR("unlink");
	socketfd = make_socket(name, &addr);
	if (bind(socketfd, (struct sockaddr *)&addr, SUN_LEN(&addr)) < 0)
		ERR("bind");
	if (listen(socketfd, BACKLOG) < 0)
		ERR("listen");
	return socketfd;
}

int add_new_client(int sfd)
{
	int nfd;
	if ((nfd = TEMP_FAILURE_RETRY(accept(sfd, NULL, NULL))) < 0) {
		if (EAGAIN == errno || EWOULDBLOCK == errno)
			return -1;
		ERR("accept");
	}
	return nfd;
}

void usage(char *name)
{
	fprintf(stderr, "USAGE: %s socket port\n", name);
}

ssize_t bulk_read(int fd, char *buf, size_t count)
{
	int c;
	size_t len = 0;
	do {
		c = TEMP_FAILURE_RETRY(read(fd, buf, count));
		if (c < 0)
			return c;
		if (0 == c)
			return len;
		buf += c;
		len += c;
		count -= c;
	} while (count > 0);
	return len;
}

ssize_t bulk_write(int fd, char *buf, size_t count)
{
	int c;
	size_t len = 0;
	do {
		c = TEMP_FAILURE_RETRY(write(fd, buf, count));
		if (c < 0)
			return c;
		buf += c;
		len += c;
		count -= c;
	} while (count > 0);
	return len;
}

void calculate(int32_t data[5])
{
	int32_t op1, op2, result, status = 1;
	op1 = ntohl(data[0]);
	op2 = ntohl(data[1]);
	switch ((char)ntohl(data[3])) {
	case '+':
		result = op1 + op2;
		break;
	case '-':
		result = op1 - op2;
		break;
	case '*':
		result = op1 * op2;
		break;
	case '/':
		if (!op2)
			status = 0;
		else
			result = op1 / op2;
		break;
	default:
		status = 0;
	}
	data[4] = htonl(status);
	data[2] = htonl(result);
}

void doServer(int fdL)
{
	int cfd;
	int32_t data[5];
	ssize_t size;
	fd_set base_rfds, rfds;
	sigset_t mask, oldmask;
	FD_ZERO(&base_rfds);
	FD_SET(fdL, &base_rfds);
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigprocmask(SIG_BLOCK, &mask, &oldmask);
	while (do_work) {
		rfds = base_rfds;
		if (pselect(fdL + 1, &rfds, NULL, NULL, NULL, &oldmask) > 0) {
			if ((cfd = add_new_client(fdL)) >= 0) {
				;
				if ((size = bulk_read(cfd, (char *)data, sizeof(int32_t[5]))) < 0)
					ERR("read:");
				if (size == (int)sizeof(int32_t[5])) {
					calculate(data);
					if (bulk_write(cfd, (char *)data, sizeof(int32_t[5])) < 0 && errno != EPIPE)
						ERR("write:");
				}
				if (TEMP_FAILURE_RETRY(close(cfd)) < 0)
					ERR("close");
			}
		} else {
			if (EINTR == errno)
				continue;
			ERR("pselect");
		}
	}
	sigprocmask(SIG_UNBLOCK, &mask, NULL);
}

int main(int argc, char **argv)
{
	int fdL;
	int new_flags;
	if (argc != 3) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}
	if (sethandler(SIG_IGN, SIGPIPE))
		ERR("Seting SIGPIPE:");
	if (sethandler(sigint_handler, SIGINT))
		ERR("Seting SIGINT:");
	fdL = bind_socket(argv[1]);
	new_flags = fcntl(fdL, F_GETFL) | O_NONBLOCK;
	fcntl(fdL, F_SETFL, new_flags);
	doServer(fdL);
	if (TEMP_FAILURE_RETRY(close(fdL)) < 0)
		ERR("close");
	if (unlink(argv[1]) < 0)
		ERR("unlink");
	fprintf(stderr, "Server has terminated.\n");
	return EXIT_SUCCESS;
}
