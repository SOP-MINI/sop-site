#include <stdio.h>
#include <unistd.h>

int main(int argc, const char **argv) {
  printf("Hello ");
  write(STDOUT_FILENO, "my ", 3);
  printf("friend!");
  // TODO: What will be actually printed?
  return 0;
}
