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

  sa.sa_handler = handler;

  int ret = sigaction(SIGINT, &sa, NULL);
  if (ret < 0) {
    perror("sigaction(): ");
    return 1;
  }
  ret = sigaction(SIGQUIT, &sa, NULL);
  if (ret < 0) {
    perror("sigaction(): ");
    return 1;
  }

  sigset_t mask, old_mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGQUIT);
  sigprocmask(SIG_BLOCK, &mask, &old_mask);

  while (1) {
    printf("> ");
    fflush(stdout);
    int c = -1;
    while (c == -1) {
      c = getchar();
    }
    sigprocmask(SIG_SETMASK, &old_mask, &mask);
    old_mask = mask;
  }

  return 1;
}
