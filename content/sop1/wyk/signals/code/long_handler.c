#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void handler(int sig) {
  static volatile sig_atomic_t id = 0;
  char fingerprint = 'a' + (id++ % ('z' - 'a' + 1));

  char enter_msg[] = "[?] Entering signal ? handler\n";
  enter_msg[1] = fingerprint;
  enter_msg[20] = '0' + sig;
  write(STDOUT_FILENO, enter_msg, sizeof(enter_msg) - 1);

  char in_msg[] = "[?] In signal ? handler\n";
  in_msg[1] = fingerprint;
  in_msg[14] = '0' + sig;
  for (int i = 0; i < 5; ++i) {
    write(STDOUT_FILENO, in_msg, sizeof(in_msg) - 1);
    sleep(1);
  }

  char exit_msg[] = "[?] Exiting signal ? handler\n";
  exit_msg[1] = fingerprint;
  exit_msg[19] = '0' + sig;
  write(STDOUT_FILENO, exit_msg, sizeof(exit_msg) - 1);
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
