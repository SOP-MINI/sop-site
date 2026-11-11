#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

void sigchld_handler(int sig) {
  (void)sig;
  int saved_errno = errno;
  const char msg[] = "SIGCHLD!\n";
  write(STDOUT_FILENO, msg, sizeof(msg) - 1);

  pid_t pid;
  for (;;) {
    pid = waitpid(0, NULL, WNOHANG);
    if (0 == pid)
      break;
    if (0 >= pid) {
      if (ECHILD == errno)
        break;
      const char err_msg[] = "waitpid error\n";
      write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
      _exit(1);
    }
  }
  errno = saved_errno;
}

void sigint_handler(int sig) {
  (void)sig;
  int saved_errno = errno;
  const char msg[] = "SIGINT!\n";
  write(STDOUT_FILENO, msg, sizeof(msg) - 1);
  errno = saved_errno;
}

int main(int argc, const char **argv) {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));

  sa.sa_handler = sigchld_handler;
  int ret = sigaction(SIGCHLD, &sa, NULL);
  if (ret < 0) {
    perror("sigaction(): ");
    return 1;
  }

  sa.sa_handler = sigint_handler;
  ret = sigaction(SIGINT, &sa, NULL);
  if (ret < 0) {
    perror("sigaction(): ");
    return 1;
  }

  for (int i = 0; i < 3; ++i) {
    switch (fork()) {
    case -1:
      perror("fork()");
      exit(1);
    case 0:
      sleep(2 * (i + 1));
      printf("Child [%d] exiting!\n", getpid());
      exit(0);
    default:
      break;
    }
  }

  while (1) {
    char c;
    ssize_t ret = read(STDIN_FILENO, &c, 1);
    if (ret < 0) {
      perror("read()");
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

  while(wait(NULL) > 0);
  return 1;
}
