#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void handler(int sig) { printf("\n\n[%d] Signal %d!\n\n", getpid(), sig); }

int main(int argc, const char **argv) {
  printf("[%d] Hello, world!\n", getpid());

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));

  sa.sa_handler = handler; // TODO: SIG_IGN
  int ret = sigaction(SIGINT, &sa, NULL);
  if (ret < 0) {
    perror("sigaction(): ");
    return 1;
  }

  sigset_t mask, old_mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigprocmask(SIG_BLOCK, &mask, &old_mask);

  while (1) {
    printf("loop\n");
    int sig;
    sigwait(&mask, &sig);
    printf("Signal %d received\n", sig);
  }

  return 1;
}
