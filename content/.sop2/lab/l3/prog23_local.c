#define _GNU_SOURCE
#include <errno.h>
#include <netdb.h>
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

int connect_socket(char *name)
{
    struct sockaddr_un addr;
    int socketfd;
    socketfd = make_socket(name, &addr);
    if (connect(socketfd, (struct sockaddr *)&addr, SUN_LEN(&addr)) < 0)
    {
        if (errno != EINTR)
            ERR("connect");
        else
        {
            fd_set wfds;
            int status;
            socklen_t size = sizeof(int);
            FD_ZERO(&wfds);
            FD_SET(socketfd, &wfds);
            if (TEMP_FAILURE_RETRY(select(socketfd + 1, NULL, &wfds, NULL, NULL)) < 0)
                ERR("select");
            if (getsockopt(socketfd, SOL_SOCKET, SO_ERROR, &status, &size) < 0)
                ERR("getsockopt");
            if (0 != status)
                ERR("connect");
        }
    }
    return socketfd;
}

void usage(char *name) { fprintf(stderr, "USAGE: %s socket operand1 operand2 operation \n", name); }

ssize_t bulk_read(int fd, char *buf, size_t count)
{
    int c;
    size_t len = 0;
    do
    {
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
    do
    {
        c = TEMP_FAILURE_RETRY(write(fd, buf, count));
        if (c < 0)
            return c;
        buf += c;
        len += c;
        count -= c;
    } while (count > 0);
    return len;
}

void prepare_request(char **argv, int32_t data[5])
{
    data[0] = htonl(atoi(argv[2]));
    data[1] = htonl(atoi(argv[3]));
    data[2] = htonl(0);
    data[3] = htonl((int32_t)(argv[4][0]));
    data[4] = htonl(1);
}

void print_answer(int32_t data[5])
{
    if (ntohl(data[4]))
        printf("%d %c %d = %d\n", ntohl(data[0]), (char)ntohl(data[3]), ntohl(data[1]), ntohl(data[2]));
    else
        printf("Operation impossible\n");
}

int main(int argc, char **argv)
{
    int fd;
    int32_t data[5];
    if (argc != 5)
    {
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    if (sethandler(SIG_IGN, SIGPIPE))
        ERR("Seting SIGPIPE:");
    fd = connect_socket(argv[1]);
    prepare_request(argv, data);
    /* Broken PIPE is treated as critical error here */
    if (bulk_write(fd, (char *)data, sizeof(int32_t[5])) < 0)
        ERR("write:");
    if (bulk_read(fd, (char *)data, sizeof(int32_t[5])) < (int)sizeof(int32_t[5]))
        ERR("read:");
    print_answer(data);
    if (TEMP_FAILURE_RETRY(close(fd)) < 0)
        ERR("close");
    return EXIT_SUCCESS;
}
