#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {
  char buff[10] = {};
  for (int i = 0; i < 10000000; ++i) {
    if (i % 15 == 0) {
      printf("FizzBuzz ");
    } else if (i % 3) {
      printf("Fizz ");
    } else if (i % 5) {
      printf("Buzz ");
    } else {
      int n = snprintf(buff, sizeof(buff), "%d ", i);
      printf("%s ", buff);
    }
  }
  return EXIT_SUCCESS;
}
