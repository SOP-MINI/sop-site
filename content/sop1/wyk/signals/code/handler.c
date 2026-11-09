#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define STR(s) #s
#define N 3
#ifndef SIG
#define SIG SIGINT
#endif

void handler(int sig) {
  printf("\n=== PID %d RECEIVED SIG %d ===\n\n", getpid(), sig);
}

void install_handler() {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));

  sa.sa_handler = handler;

  int ret = sigaction(SIG, &sa, NULL);
  if (ret < 0) {
    perror("sigaction(): ");
    exit(1);
  }
}

int main(int argc, const char **argv) {
  install_handler();

  printf("[%d:%d] Hello from parent!\n", getpid(), getpgid(0));

  pid_t pids[N];

  for (int i = 0; i < N; ++i) {
    switch (pids[i] = fork()) {
    case -1:
      kill(0, SIGKILL); // Note 0 here
      exit(1);
    case 0:
      printf("[%d:%d] Hello from child %d!\n", getpid(), getpgid(0), i);
      while (1) {
        sleep(10);
        printf("[%d:%d] Child %d working...\n", getpid(), getpgid(0), i);
      }
      printf("[%d:%d] Bye from child %d!\n", getpid(), getpgid(0), i);
      exit(0);
    default:
      break;
    }
  }

  pid_t pid;
  while (scanf("%d", &pid) == 1) {
    int ret = kill(pid, SIG);
    printf("Sending " STR(SIG) " to PID %d returned %d\n", pid, ret);
  }

  printf("Wrong input!\n");
  kill(0, SIGKILL);
  while (wait(NULL) > 0)
    ;
  return 0;
}
