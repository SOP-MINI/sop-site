#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {
  char buff[10] = {};
  for (int i = 0; i < 10000000; ++i) {
    if (i % 15 == 0) {
      int ret = write(STDOUT_FILENO, "FizzBuzz ", 9);
      (void)ret;
    } else if (i % 3) {
      int ret = write(STDOUT_FILENO, "Fizz ", 5);
      (void)ret;
    } else if (i % 5) {
      int ret = write(STDOUT_FILENO, "Buzz ", 5);
      (void)ret;
    } else {
      int n = snprintf(buff, sizeof(buff), "%d ", i);
      int ret = write(STDOUT_FILENO, buff, n);
      (void)ret;
    }
  }
  return EXIT_SUCCESS;
}
