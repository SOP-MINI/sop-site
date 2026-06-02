#define _GNU_SOURCE
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

int make_socket(int domain, int type)
{
    int sock;
    sock = socket(domain, type, 0);
    if (sock < 0)
        ERR("socket");
    return sock;
}

int bind_inet_socket(uint16_t port, int type, int backlog)
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
        if (listen(socketfd, backlog) < 0)
            ERR("listen");
    return socketfd;
}

#define MSG_MAX 64
#define USERS 10
#define THREADS 8
#define LOGIN_LEN 16
#define COMMAND_LEN 8
#define HEADER_LEN (LOGIN_LEN + COMMAND_LEN)
#define COMMANDS_NUM 6

static char *LOGINS[USERS] = {"kaczmarskik", "jelowickif", "hermant",  "turs",     "krasowskip",
                              "larysaz",     "zygulas",    "homendaw", "galazkap", "jastrzebskaaaaaa"};
static char *COMMANDS[COMMANDS_NUM] = {"RUN", "EXIT", "PAUSE", "COMPUTE", "LIST", "GATHER"};

typedef enum
{
    RUN,
    EXIT,
    PAUSE,
    COMPUTE,
    LIST,
    GATHER,
} Command;

void usage(const char *name)
{
    printf("%s <in_port>\n", name);
    printf("  in_port - port that accepts messages\n");
    exit(EXIT_FAILURE);
}

typedef struct
{
    char login[LOGIN_LEN];
    char command[COMMAND_LEN];
    uint32_t params[10];
    int params_count;
} Message;

bool check_login(const char login[])
{
    for (int i = 0; i < USERS; i++)
    {
        if (strncmp(login, LOGINS[i], LOGIN_LEN) == 0)
            return true;
    }
    return false;
}

int check_command(const char command[])
{
    for (int i = 0; i < COMMANDS_NUM; i++)
    {
        if (strncmp(command, COMMANDS[i], COMMAND_LEN) == 0)
            return i;
    }
    return -1;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        usage(argv[0]);
    int socket = bind_inet_socket(atoi(argv[1]), SOCK_DGRAM, 1);

    Message m;
    while (true)
    {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        ssize_t m_size = recvfrom(socket, &m, MSG_MAX + 1, 0, (struct sockaddr *)&addr, &len);
        if (m_size < HEADER_LEN || m_size > MSG_MAX)
        {
            printf("error: wrong message length %ld\n", m_size);
            continue;
        }
        if (!check_login(m.login))
        {
            printf("error: wrong login %.16s\n", m.login);
            continue;
        }
        const int command_idx = check_command(m.command);
        switch (command_idx)
        {
            case -1:
                printf("error: unknown command %.8s\n", m.command);
                continue;
                break;
            case EXIT:
                close(socket);
                return 0;
            case COMPUTE:
                if ((m_size - HEADER_LEN) % (2 * sizeof(uint32_t)) != 0)
                {
                    printf("error: wrong compute message params length %ld\n", m_size - HEADER_LEN);
                    continue;
                }
                m.params_count = (m_size - HEADER_LEN) / sizeof(uint32_t);
                printf("%.16s: %.8s ", m.login, m.command);
                for (int i = 0; i < m.params_count; i++)
                {
                    m.params[i] = ntohl(m.params[i]);
                    printf("%u ", m.params[i]);
                }
                putchar('\n');
                break;
            default:
                printf("%.16s: %.8s\n", m.login, m.command);
                break;
        }
    }

    close(socket);
    return 0;
}
