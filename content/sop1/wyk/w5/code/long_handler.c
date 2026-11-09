#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void handler(int sig) {
  char fingerprint = rand() % ('z' - 'a') + 'a';
  printf("[%c] Entering signal %d handler\n", fingerprint, sig);
  for(int i = 0; i < 5; ++i) {
    printf("[%c] In signal %d handler\n", fingerprint, sig);
    sleep(1);
  }
  printf("[%c] Exiting signal %d handler\n", fingerprint, sig);
}

int main(int argc, const char** argv)
{
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));

  sa.sa_handler = handler; // TODO: SIG_IGN
  // sa.sa_flags = SA_NODEFER;

  int ret = sigaction(SIGINT, &sa, NULL);
  if (ret < 0)
  {
    perror("sigaction(): ");
    return 1;
  }

  ret = sigaction(SIGQUIT, &sa, NULL);
  if (ret < 0)
  {
    perror("sigaction(): ");
    return 1;
  }

  while (1)
  {
    write(STDOUT_FILENO, ".", 1);
    sleep(1);
  }

  return 1;
}
