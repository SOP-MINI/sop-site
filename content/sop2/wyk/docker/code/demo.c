#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

void handle_signal(int sig) {
    fprintf(stderr, "\nsignal %d! exiting...\n", sig);
    exit(0);
}

void demo_sleep(const char* arg){
    (void)arg;
    while (1)
    {
        printf("sleeping...\n");
        sleep(3);
    }
}

void demo_kill(const char* arg)
{
    pid_t target_pid = atoi(arg);
    printf("attempting to send signal 0 to PID %d... ", target_pid);
    if (kill(target_pid, 0) == -1) {
        printf("failed!\n");
        perror("kill()");
    } else {
        printf("done!\n");
    }
}

void demo_fork(const char* arg) {
    (void)arg;
    pid_t pid;
    switch (pid = fork())
    {
        case -1:
            perror("fork()");
            break;
        case 0:
            printf("hello from child! ppid = %d\n", getppid());
            sleep(3);
            exit(0);
        default:
            printf("created child pid = %d\n", pid);
            wait(NULL);
    }
}

void demo_if(const char* arg) {
    (void)arg;
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    printf(" -> Available network interfaces:\n");
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;
        if (ifa->ifa_addr->sa_family == AF_INET) {
            printf("    - %s\n", ifa->ifa_name);
        }
    }
    freeifaddrs(ifaddr);
}

void demo_udp(const char* arg) {
    (void)arg;
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket()");
        return;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
        printf("bound UDP port 8080!\n");
    } else {
        perror("bind()");
        close(fd);
        return;
    }

    char buffer[1024];
    struct sockaddr_in sender_addr;
    socklen_t sender_len = sizeof(sender_addr);

    // Server loop
    while (1) {
        ssize_t bytes_read = recvfrom(fd, buffer, sizeof(buffer) - 1, 0,
                                      (struct sockaddr *)&sender_addr, &sender_len);

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            buffer[strcspn(buffer, "\n")] = 0;

            char sender_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(sender_addr.sin_addr), sender_ip, INET_ADDRSTRLEN);

            printf(" -> received %zd bytes from %s:%d: '%s'\n",
                   bytes_read, sender_ip, ntohs(sender_addr.sin_port), buffer);
        } else if (bytes_read < 0) {
            perror("recvfrom()");
            break;
        }
    }
    close(fd);
}

void demo_list(const char* arg) {
    const char* dir = arg ? arg : "/tmp";
    printf("contents of '%s':\n", dir);
    DIR *d = opendir("/tmp");
    if (d) {
        struct dirent *ent;
        while ((ent = readdir(d)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                printf("    - %s\n", ent->d_name);
            }
        }
        closedir(d);
    }
}

void demo_write(const char* arg) {
    const char *fpath = arg ? arg : "/tmp/container_secret.txt";

    FILE *f = fopen(fpath, "w");
    if (f) {
        fprintf(f, "pid = %d\n", getpid());
        fclose(f);
        printf("written '%s'\n", fpath);
    } else {
        perror("fopen()");
    }

    printf("waiting for signal...\n");
    pause();
}

void demo_mem(const char* arg) {
    (void)arg;
    size_t allocated = 0;
    size_t chunk_size = 10 * 1024 * 1024; // 10 MB

    while (1) {
        void *p = malloc(chunk_size);
        if (!p) {
            perror("malloc failed");
            break;
        }
        // KEY: memset forces physical page allocation (Page Fault).
        // malloc alone only reserves virtual memory (VMA), which wouldn't trigger the OOM Killer!
        memset(p, 0xAA, chunk_size);
        allocated += chunk_size;
        printf("allocated %zu MB\n", allocated / (1024 * 1024));
        usleep(100000);
    }
}

// volatile prevents compiler from removing the infinite loop
volatile unsigned long long counter = 0;

void demo_cpu(const char* arg) {
    (void)arg;
    printf("running CPU spinloop...\n");
    while (1) {
        counter++;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [ex] --[kill|fork|sleep|if|udp|list|write|mem|cpu] [arg]\n", argv[0]);
        return 1;
    }

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    printf("my pid = %d\n", getpid());
    printf("running example '%s'\n", argv[1]);
    if (strcmp(argv[1], "--kill") == 0) demo_kill(argc > 2 ? argv[2] : NULL);
    else if (strcmp(argv[1], "--fork") == 0) demo_fork(argc > 2 ? argv[2] : NULL);
    else if (strcmp(argv[1], "--sleep") == 0) demo_sleep(argc > 2 ? argv[2] : NULL);
    else if (strcmp(argv[1], "--if") == 0) demo_if(argc > 2 ? argv[2] : NULL);
    else if (strcmp(argv[1], "--udp") == 0) demo_udp(argc > 2 ? argv[2] : NULL);
    else if (strcmp(argv[1], "--list")  == 0) demo_list(argc > 2 ? argv[2] : NULL);
    else if (strcmp(argv[1], "--write")  == 0) demo_write(argc > 2 ? argv[2] : NULL);
    else if (strcmp(argv[1], "--mem") == 0) demo_mem(argc > 2 ? argv[2] : NULL);
    else if (strcmp(argv[1], "--cpu") == 0) demo_cpu(argc > 2 ? argv[2] : NULL);
    else printf("Unknown example: %s\n", argv[1]);

    return 0;
}