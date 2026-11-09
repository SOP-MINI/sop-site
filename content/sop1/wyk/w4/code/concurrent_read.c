#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {

  FILE *f = stdin;

  int ret = fork();

  char buf[10] = {0};

  while (!feof(f)) {
    fscanf(f, "%9s", buf);
    printf("[%d] => %s\n", getpid(), buf);
  }

  printf("[%d] exiting\n", getpid());

  return 0;
}
