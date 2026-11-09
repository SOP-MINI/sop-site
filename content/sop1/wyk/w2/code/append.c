#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Filename required!\n");
    return 1;
  }
  const char *fn = argv[1];
  int fd = open(fn, O_RDWR | O_APPEND);
  if (fd < 0) {
    printf("open() failed: %s", strerror(errno));
    return 1;
  }

  char c = 0;
  ssize_t ret = 0;
  ret = read(fd, &c, 1);
  printf("read(%d, _, 1) returned %ld, c = '%c'\n", fd, ret, c);
  ret = read(fd, &c, 1);
  printf("read(%d, _, 1) returned %ld, c = '%c'\n", fd, ret, c);

  c = '!';
  ret = write(fd, &c, 1);
  printf("write(%d, '%c', 1) returned %ld\n", fd, c, ret);

  ret = read(fd, &c, 1);
  printf("read(%d, _, 1) returned %ld, c = '%c'\n", fd, ret, c);
  ret = read(fd, &c, 1);
  printf("read(%d, _, 1) returned %ld, c = '%c'\n", fd, ret, c);

  close(fd);
  return 0;
}
