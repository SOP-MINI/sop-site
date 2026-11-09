#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void handler(int sig) { printf("\n\n[%d] Signal %d!\n\n", getpid(), sig); }

int main(int argc, const char **argv) {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));

  sa.sa_handler = handler;
  // sa.sa_flags = SA_RESTART; // TODO: Uncomment

  int ret = sigaction(SIGINT, &sa, NULL);
  if (ret < 0) {
    perror("sigaction(): ");
    return 1;
  }

  while (1) {
    char c;
    ssize_t ret = read(STDIN_FILENO, &c, 1);
    if (ret < 0) {
      perror("read()");
      if (errno != EINTR) {
        break;
      }
    } else if (ret == 0) {
      printf("read() returned 0 bytes\n");
      break;
    } else {
      if (c == '\n')
        printf("read() returned '\\n'\n");
      else
        printf("read() returned '%c'\n", c);
    }
  }

  return 1;
}
