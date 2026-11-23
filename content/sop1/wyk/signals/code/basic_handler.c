#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>

void handler(int sig) {
  (void)sig;
  const char msg[] = "\n\nSignal received!\n\n";
  write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

int main(int argc, const char** argv)
{
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));

  sa.sa_handler = handler; // TODO: SIG_IGN

  int ret = sigaction(SIGINT, &sa, NULL);
  if (ret < 0)
  {
    perror("sigaction(): ");
    return 1;
  }

  // fork(); // TODO: uncomment
  printf("[%d] Hello, world!\n", getpid());

  while (1)
  {
    write(STDOUT_FILENO, ".", 1);
    sleep(1);
  }

  return 1;
}
