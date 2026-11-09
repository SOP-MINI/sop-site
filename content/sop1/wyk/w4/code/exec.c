#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  printf("[%d] Calling fork()!\n", getpid());
  int ret = fork();
  switch (ret) {
  case -1:
    printf("fork() failed :(\n");
    return 123;
  case 0: {
    char *args[] = {"hi!", NULL};
    char *envp[] = {"OPS=I'm here", "HOME=/tmp", NULL};
    printf("[%d] Calling exec()!\n", getpid());
    execve("./env", args, envp);
    // TODO: When following code will be executed?
    printf("exec() returned an error! %s\n", strerror(errno));
    exit(1);
  }
  default:
    break;
  }
  int status;
  wait(&status);
  printf("[%d] Child process returned status %d\n", getpid(),
         WEXITSTATUS(status));
  return 0;
}
