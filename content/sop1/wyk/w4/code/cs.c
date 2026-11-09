#include <unistd.h>

int main(int argc, char **argv) {
  for (int i = 0; i < 10; ++i) {
    sleep(1);
    // for (int j = 0; j < 1000000000; ++j);
  }
  return 0;
}
