
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define STR(s) #s
#define N 3
#ifndef SIG
#define SIG SIGKILL
#endif

int main(int argc, const char** argv)
{
  printf("[%d:%d] Hello from parent!\n", getpid(), getpgid(0));

  pid_t pids[N];

  for (int i = 0; i < N; ++i) {
    switch(pids[i] = fork()) {
    case -1:
      kill(0, SIGKILL); // Note 0 here
      exit(1);
    case 0:
      printf("[%d:%d] Hello from child %d!\n", getpid(), getpgid(0), i);
      while(1) {
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
  while (scanf("%d", &pid) == 1)
  {
    int ret = kill(pid, SIG);
    printf("Sending " STR(SIG) " to PID %d returned %d\n", pid, ret);
  }

  printf("Wrong input!\n");
  kill(0, SIGKILL);
  while(wait(NULL) > 0);
  return 0;
}

