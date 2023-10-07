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
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

#define MAXBUF 576
volatile sig_atomic_t last_signal = 0;

void sigalrm_handler(int sig) { last_signal = sig; }

int sethandler(void (*f)(int), int sigNo)
{
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = f;
    if (-1 == sigaction(sigNo, &act, NULL))
        return -1;
    return 0;
}

int make_socket(void)
{
    int sock;
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        ERR("socket");
    return sock;
}

struct sockaddr_in make_address(char *address, char *port)
{
    int ret;
    struct sockaddr_in addr;
    struct addrinfo *result;
    struct addrinfo hints = {};
    hints.ai_family = AF_INET;
    if ((ret = getaddrinfo(address, port, &hints, &result)))
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }
    addr = *(struct sockaddr_in *)(result->ai_addr);
    freeaddrinfo(result);
    return addr;
}

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

void usage(char *name) { fprintf(stderr, "USAGE: %s domain port file \n", name); }

void sendAndConfirm(int fd, struct sockaddr_in addr, char *buf1, char *buf2, ssize_t size)
{
    struct itimerval ts;
    if (TEMP_FAILURE_RETRY(sendto(fd, buf1, size, 0, &addr, sizeof(addr))) < 0)
        ERR("sendto:");
    memset(&ts, 0, sizeof(struct itimerval));
    ts.it_value.tv_usec = 500000;
    setitimer(ITIMER_REAL, &ts, NULL);
    last_signal = 0;
    while (recv(fd, buf2, size, 0) < 0)
    {
        if (EINTR != errno)
            ERR("recv:");
        if (SIGALRM == last_signal)
            break;
    }
}

void doClient(int fd, struct sockaddr_in addr, int file)
{
    char buf[MAXBUF];
    char buf2[MAXBUF];
    int offset = 2 * sizeof(int32_t);
    int32_t chunkNo = 0;
    int32_t last = 0;
    ssize_t size;
    int counter;
    do
    {
        if ((size = bulk_read(file, buf + offset, MAXBUF - offset)) < 0)
            ERR("read from file:");
        *((int32_t *)buf) = htonl(++chunkNo);
        if (size < MAXBUF - offset)
        {
            last = 1;
            memset(buf + offset + size, 0, MAXBUF - offset - size);
        }
        *(((int32_t *)buf) + 1) = htonl(last);
        memset(buf2, 0, MAXBUF);
        counter = 0;
        do
        {
            counter++;
            sendAndConfirm(fd, addr, buf, buf2, MAXBUF);
        } while (*((int32_t *)buf2) != htonl(chunkNo) && counter <= 5);
        if (*((int32_t *)buf2) != htonl(chunkNo) && counter > 5)
            break;
    } while (size == MAXBUF - offset);
}

int main(int argc, char **argv)
{
    int fd, file;
    struct sockaddr_in addr;
    if (argc != 4)
    {
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    if (sethandler(SIG_IGN, SIGPIPE))
        ERR("Seting SIGPIPE:");
    if (sethandler(sigalrm_handler, SIGALRM))
        ERR("Seting SIGALRM:");
    if ((file = TEMP_FAILURE_RETRY(open(argv[3], O_RDONLY))) < 0)
        ERR("open:");
    fd = make_socket();
    addr = make_address(argv[1], argv[2]);
    doClient(fd, addr, file);
    if (TEMP_FAILURE_RETRY(close(fd)) < 0)
        ERR("close");
    if (TEMP_FAILURE_RETRY(close(file)) < 0)
        ERR("close");
    return EXIT_SUCCESS;
}
