#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int set_nonblock(int desc) {
  int oldflags = fcntl(desc, F_GETFL, 0);
  if (oldflags == -1)
    return -1;
  oldflags |= O_NONBLOCK;
  return fcntl(desc, F_SETFL, oldflags);
}


int main(int argc, const char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Filename required!\n");
    return 1;
  }

  int fd;

  if (strcmp(argv[1], "stdin") == 0) {
    fd = STDIN_FILENO;
#ifdef NONBLOCK
    set_nonblock(fd);
#endif
  } else {
#ifndef NONBLOCK
    fd = open(argv[1], O_RDONLY);
#else
    fd = open(argv[1], O_RDONLY | O_NONBLOCK);
#endif
    if (fd < 0) {
      perror("open(): ");
      return 1;
    }
    printf("open() returned %d\n", fd);
  }

  while (1) {
    char buf[4] = {};
    ssize_t ret = read(fd, buf, sizeof(buf));
    if (ret < 0) {
      perror("read(): ");
    } else if (ret == 0) {
      printf("read 0 bytes\n");
      break;
    } else {
      printf("read %ld bytes: '%c%c%c%c'\n", ret, buf[0], buf[1], buf[2],
             buf[3]);
    }
    sleep(1);
  }

  close(fd);
  return 0;
}
