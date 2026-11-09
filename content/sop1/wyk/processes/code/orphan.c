#include <stdio.h>
#include <stdlib.h>

#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  pid_t ret = fork();
  switch (ret) {
  case -1:
    printf("fork() returned error!\n");
    return 123;
  case 0:
    printf("[%d] Hello, world from child! My parent is %d\n", getpid(),
           getppid());
    sleep(5);
    printf("[%d] Bye from child! My parent is %d\n", getpid(), getppid());
    exit(100);
  default:
    printf("[%d] Hello, world from parent! My parent is %d\n", getpid(),
           getppid());
    break;
  }
  printf("[%d] bye!\n", getpid());
  return 5;
}
