#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

ssize_t bulk_read(int fd, char *buf, size_t count) {
  ssize_t c;
  ssize_t len = 0;
  do {
    c = TEMP_FAILURE_RETRY(read(fd, buf, count));
    if (c < 0)
      return c;
    if (c == 0)
      return len; // EOF
    buf += c;
    len += c;
    count -= c;
  } while (count > 0);
  return len;
}

void handler(int sig) {
  printf("\n=== PID %d RECEIVED SIG %d ===\n\n", getpid(), sig);
}

void install_handler() {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));

  sa.sa_handler = handler;

  int ret = sigaction(SIGINT, &sa, NULL);
  if (ret < 0) {
    perror("sigaction(): ");
    exit(1);
  }
}

int main(int argc, const char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Filename required!\n");
    return 1;
  }

  install_handler();

  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    perror("open(): ");
    return 1;
  }
  printf("open() returned %d\n", fd);

  while (1) {
    char buf[4] = {};
    ssize_t ret = read(fd, buf, sizeof(buf)); // TODO: replace with bulk_* version
    if (ret < 0) {
      perror("read(): ");
    } else {
      printf("read %ld bytes: '%02x%02x%02x%02x'\n", ret, buf[0], buf[1], buf[2],
             buf[3]);
    }
  }

  close(fd);
  return 0;
}
