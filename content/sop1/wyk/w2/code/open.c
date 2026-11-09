#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void test(const char *fn, const char *txt_flags, int flags) {
  printf("[test '%s']\n", txt_flags);
  int fd = open(fn, flags);
  printf("open(%s, %s) returned %d", fn, txt_flags, fd);
  if (fd < 0) {
    printf(": %s", strerror(errno));
  }
  printf("\n");
  if (fd >= 0) {
    char c = 'x';
    ssize_t ret = write(fd, &c, 1);
    printf("write(%d, '%c', 1) returned %ld\n", fd, c, ret);
    c = 0;
    /*ssize_t*/ ret = read(fd, &c, 1);
    printf("read(%d, _, 1) returned %ld, c = '%c'\n", fd, ret, c);
    close(fd);
  }
}

int main(int argc, const char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Filename required!\n");
    return 1;
  }
  const char *fn = argv[1];
  test(fn, "0", 0);
  test(fn, "O_RDONLY", O_RDONLY);
  test(fn, "O_WRONLY", O_WRONLY);
  test(fn, "O_RDWR", O_RDWR);
  return 0;
}
