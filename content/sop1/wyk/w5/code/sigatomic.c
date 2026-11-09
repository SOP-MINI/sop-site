#define _GNU_SOURCE
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

struct long_struct {
  long long a;
  long long b;
};

const struct long_struct zeros = {0, 0};
const struct long_struct ones = {1, 1};

struct long_struct flag;

//volatile sig_atomic_t flag;

void handler(int sig) {
  printf("[%d] Received signal %d! - a = %lld, b = %lld\n", getpid(), sig,
         flag.a, flag.b);
}

int main(int argc, const char **argv) {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));

  sa.sa_handler = handler;
  int ret = sigaction(SIGINT, &sa, NULL);
  if (ret < 0) {
    perror("sigaction(): ");
    return 1;
  }

  while (1) {
    flag = ones;
    flag = zeros;
  }

  return 0;
}
