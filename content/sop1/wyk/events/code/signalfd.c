#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/types.h>

int main() {
    printf("pid = %ld\n", (long)getpid());

    // Block awaited signals
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    // Create signalfd
    int sfd = signalfd(-1, &mask, /*SFD_NONBLOCK*/0);
    if (sfd == -1) {
        perror("signalfd");
        exit(EXIT_FAILURE);
    }

    while (1) {
        struct signalfd_siginfo fdsi;
        ssize_t s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
        if (s != sizeof(struct signalfd_siginfo)) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        printf("received signal %d from process %d\n", fdsi.ssi_signo, fdsi.ssi_pid);

        if (fdsi.ssi_signo == SIGTERM) {
            break;
        }

        // sleep(1);
    }

    close(sfd);
    return 0;
}
