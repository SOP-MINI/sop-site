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
#define MAXBUF 576
#define MAXADDR 5

struct connections
{
    int free;
    int32_t chunkNo;
    struct sockaddr_in addr;
};

int sethandler(void (*f)(int), int sigNo)
{
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = f;
    if (-1 == sigaction(sigNo, &act, NULL))
        return -1;
    return 0;
}

int make_socket(int domain, int type)
{
    int sock;
    sock = socket(domain, type, 0);
    if (sock < 0)
        ERR("socket");
    return sock;
}

int bind_inet_socket(uint16_t port, int type)
{
    struct sockaddr_in addr;
    int socketfd, t = 1;
    socketfd = make_socket(PF_INET, type);
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(t)))
        ERR("setsockopt");
    if (bind(socketfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        ERR("bind");
    if (SOCK_STREAM == type)
        if (listen(socketfd, BACKLOG) < 0)
            ERR("listen");
    return socketfd;
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

int findIndex(struct sockaddr_in addr, struct connections con[MAXADDR])
{
    int i, empty = -1, pos = -1;
    for (i = 0; i < MAXADDR; i++)
    {
        if (con[i].free)
            empty = i;
        else if (0 == memcmp(&addr, &(con[i].addr), sizeof(struct sockaddr_in)))
        {
            pos = i;
            break;
        }
    }
    if (-1 == pos && empty != -1)
    {
        con[empty].free = 0;
        con[empty].chunkNo = 0;
        con[empty].addr = addr;
        pos = empty;
    }
    return pos;
}

void doServer(int fd)
{
    struct sockaddr_in addr;
    struct connections con[MAXADDR];
    char buf[MAXBUF];
    socklen_t size = sizeof(addr);
    int i;
    int32_t chunkNo, last;
    for (i = 0; i < MAXADDR; i++)
        con[i].free = 1;
    for (;;)
    {
        if (TEMP_FAILURE_RETRY(recvfrom(fd, buf, MAXBUF, 0, &addr, &size) < 0))
            ERR("read:");
        if ((i = findIndex(addr, con)) >= 0)
        {
            chunkNo = ntohl(*((int32_t *)buf));
            last = ntohl(*(((int32_t *)buf) + 1));
            if (chunkNo > con[i].chunkNo + 1)
                continue;
            else if (chunkNo == con[i].chunkNo + 1)
            {
                if (last)
                {
                    printf("Last Part %d\n%s\n", chunkNo, buf + 2 * sizeof(int32_t));
                    con[i].free = 1;
                }
                else
                    printf("Part %d\n%s\n", chunkNo, buf + 2 * sizeof(int32_t));
                con[i].chunkNo++;
            }
            if (TEMP_FAILURE_RETRY(sendto(fd, buf, MAXBUF, 0, &addr, size)) < 0)
            {
                if (EPIPE == errno)
                    con[i].free = 1;
                else
                    ERR("send:");
            }
        }
    }
}

void usage(char *name) { fprintf(stderr, "USAGE: %s port\n", name); }

int main(int argc, char **argv)
{
    int fd;
    if (argc != 2)
    {
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    if (sethandler(SIG_IGN, SIGPIPE))
        ERR("Seting SIGPIPE:");
    fd = bind_inet_socket(atoi(argv[1]), SOCK_DGRAM);
    doServer(fd);
    if (TEMP_FAILURE_RETRY(close(fd)) < 0)
        ERR("close");
    fprintf(stderr, "Server has terminated.\n");
    return EXIT_SUCCESS;
}
