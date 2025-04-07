#include "l4_common.h"

void prepare_request(char **argv, int32_t data[5])
{
    data[0] = htonl(atoi(argv[3]));
    data[1] = htonl(atoi(argv[4]));
    data[2] = htonl(0);
    data[3] = htonl((int32_t)(argv[5][0]));
    data[4] = htonl(1);
}

void print_answer(int32_t data[5])
{
    if (ntohl(data[4]))
        printf("%d %c %d = %d\n", ntohl(data[0]), (char)ntohl(data[3]), ntohl(data[1]), ntohl(data[2]));
    else
        printf("Operation impossible\n");
}

void usage(char *name) { fprintf(stderr, "USAGE: %s domain port  operand1 operand2 operation \n", name); }

int main(int argc, char **argv)
{
    int fd;
    int32_t data[5];
    if (argc != 6)
    {
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    fd = connect_tcp_socket(argv[1], argv[2]);
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
