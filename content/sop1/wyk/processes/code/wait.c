#include <stdio.h>
#include <stdlib.h>

#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  for (int i = 0; i < 3; ++i) {
    pid_t ret = fork();
    switch (ret) {
    case -1:
      printf("fork() returned error!\n");
      return 123;
    case 0:
      printf("[%d] Hello, world from child! My parent is %d\n", getpid(),
             getppid());
      sleep((i+1)*2);
      exit(10 * i);
    default:
      break;
    }
  }
  for (int i = 0; i < 3; ++i) {
    int status;
    pid_t ret = wait(&status);
    printf("[%d] wait() returned %d, status = %d\n", getpid(), ret, WEXITSTATUS(status));
  }
  printf("[%d] bye!\n", getpid());
  return 5;
}
