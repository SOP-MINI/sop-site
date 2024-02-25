#include "l4_common.h"

#define BACKLOG 3
#define MAXBUF 576
#define MAXADDR 5

struct connections
{
    int free;
    int32_t chunkNo;
    struct sockaddr_in addr;
};

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
