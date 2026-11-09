#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void handler(int sig) {
  printf("\n=== PID %d RECEIVED SIG %d ===\n\n", getpid(), sig);
}

int main(int argc, const char **argv) {

  printf("Hello, world!\n");

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));

  sa.sa_handler = handler;

  int ret = sigaction(SIGSEGV, &sa, NULL);
  if (ret < 0) {
    perror("sigaction(): ");
    return 1;
  }

  printf("Accessing NULL!\n");

  int *ptr = 0;
  *ptr = 3;

  printf("Bye, bye!\n");

  return 1;
}
