#include "l4_common.h"

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
        ERR("connect");
    }
    return socketfd;
}

void usage(char *name) { fprintf(stderr, "USAGE: %s socket operand1 operand2 operation \n", name); }

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
    fd = connect_socket(argv[1]);
    prepare_request(argv, data);
    if (bulk_write(fd, (char *)data, sizeof(int32_t[5])) < 0)
        ERR("write:");
    if (bulk_read(fd, (char *)data, sizeof(int32_t[5])) < (int)sizeof(int32_t[5]))
        ERR("read:");
    print_answer(data);
    if (TEMP_FAILURE_RETRY(close(fd)) < 0)
        ERR("close");
    return EXIT_SUCCESS;
}
