#include <stdio.h>

#include <malloc.h>
#include <unistd.h>

int main(int argc, char **argv) {
  printf("[%d] Watch out i'm calling fork()!\n", getpid());

  int* ptr = (int*)malloc(sizeof(int));
  *ptr = 3;

  int ret = fork();
  printf("[%d] fork() returned %d!\n", getpid(), ret);
  switch (ret) {
  case -1:
    printf("fork() failed :(\n");
    return 123;
  case 0:
    printf("[%d] Hello, world! I'm the child process! My parent is %d.\n",
           getpid(), getppid());
    *ptr = 5;
    break;
  default:
    printf("[%d] Hello, world! I'm the parent process! My child is %d.\n",
           getpid(), ret);
    break;
  }

  sleep(3);
  printf("[%d] ptr = %p, *ptr = %d\n", getpid(), ptr, *ptr);

  printf("[%d] Process exiting!\n", getpid());
  return 0;
}
