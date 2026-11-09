#define _GNU_SOURCE

#include <stdio.h>

#include <sched.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>

int child_main(void* arg)
{
  int* ptr = (int*)arg;
  char text[] = "This string sits in child's stack";
  char buffer[100];
  snprintf(buffer, sizeof(buffer), "[child] child begins - arg = %d\n", *ptr);
  write(STDOUT_FILENO, buffer, strlen(buffer));
  *ptr = 10;
  sleep(8);
  snprintf(buffer, sizeof(buffer), "[child] child exits\n");
  write(STDOUT_FILENO, buffer, strlen(buffer));
}

int main(int argc, char **argv) {

  int* ptr = (int*)malloc(sizeof(int));
  *ptr = 3;

  printf("[parent] Watch out i'm calling clone()!\n");

  size_t stack_size = 1024;
  char* child_stack = (char*)malloc(stack_size);

  int ret = clone(child_main, child_stack + stack_size, CLONE_VM, ptr);
  printf("[parent] clone() returned %d!\n", ret);

  sleep(5);
  printf("[parent] *ptr = %d\n", *ptr);
  sleep(5);

  printf("[parent] parent exits!\n");
  return 0;
}
