#include "l4-1_common.h"

#define BACKLOG 3
#define MAX_EVENTS 16

volatile sig_atomic_t do_work = 1;

void sigint_handler(int sig) { do_work = 0; }

void usage(char *name) { fprintf(stderr, "USAGE: %s socket port\n", name); }

void calculate(int32_t data[5])
{
    int32_t op1, op2, result = -1, status = 1;
    op1 = ntohl(data[0]);
    op2 = ntohl(data[1]);
    switch ((char)ntohl(data[3]))
    {
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

void doServer(int local_listen_socket, int tcp_listen_socket)
{
    int epoll_descriptor;
    if ((epoll_descriptor = epoll_create1(0)) < 0)
    {
        ERR("epoll_create:");
    }
    struct epoll_event event, events[MAX_EVENTS];
    event.events = EPOLLIN;
    event.data.fd = local_listen_socket;
    if (epoll_ctl(epoll_descriptor, EPOLL_CTL_ADD, local_listen_socket, &event) == -1)
    {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }

    event.data.fd = tcp_listen_socket;
    if (epoll_ctl(epoll_descriptor, EPOLL_CTL_ADD, tcp_listen_socket, &event) == -1)
    {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }

    int nfds;
    int32_t data[5];
    ssize_t size;
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);
    while (do_work)
    {
        if ((nfds = epoll_pwait(epoll_descriptor, events, MAX_EVENTS, -1, &oldmask)) > 0)
        {
            for (int n = 0; n < nfds; n++)
            {
                int client_socket = add_new_client(events[n].data.fd);
                if ((size = bulk_read(client_socket, (char *)data, sizeof(int32_t[5]))) < 0)
                    ERR("read:");
                if (size == (int)sizeof(int32_t[5]))
                {
                    calculate(data);
                    if (bulk_write(client_socket, (char *)data, sizeof(int32_t[5])) < 0 && errno != EPIPE)
                        ERR("write:");
                }
                if (TEMP_FAILURE_RETRY(close(client_socket)) < 0)
                    ERR("close");
            }
        }
        else
        {
            if (errno == EINTR)
                continue;
            ERR("epoll_pwait");
        }
    }
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
}

int main(int argc, char **argv)
{
    int local_listen_socket, tcp_listen_socket;
    int new_flags;
    if (argc != 3)
    {
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    if (sethandler(SIG_IGN, SIGPIPE))
        ERR("Seting SIGPIPE:");
    if (sethandler(sigint_handler, SIGINT))
        ERR("Seting SIGINT:");
    local_listen_socket = bind_local_socket(argv[1], BACKLOG);
    new_flags = fcntl(local_listen_socket, F_GETFL) | O_NONBLOCK;
    fcntl(local_listen_socket, F_SETFL, new_flags);
    tcp_listen_socket = bind_tcp_socket(atoi(argv[2]), BACKLOG);
    new_flags = fcntl(tcp_listen_socket, F_GETFL) | O_NONBLOCK;
    fcntl(tcp_listen_socket, F_SETFL, new_flags);
    doServer(local_listen_socket, tcp_listen_socket);
    if (TEMP_FAILURE_RETRY(close(local_listen_socket)) < 0)
        ERR("close");
    if (unlink(argv[1]) < 0)
        ERR("unlink");
    if (TEMP_FAILURE_RETRY(close(tcp_listen_socket)) < 0)
        ERR("close");
    fprintf(stderr, "Server has terminated.\n");
    return EXIT_SUCCESS;
}
