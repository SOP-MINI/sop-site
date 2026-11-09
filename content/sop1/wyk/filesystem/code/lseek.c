#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, const char **argv) {
  int fd = STDIN_FILENO;

  if (argc >= 2) {
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
      perror("open(): ");
      return 1;
    }
    printf("open() returned %d\n", fd);
  }

  char c;
  ssize_t ret;
  off_t seek_ret;

  for (int i = 0; i < 3; ++i) {
    ret = read(fd, &c, 1);
    printf("read(%d, _, 1) returned %ld, c = '%c'\n", fd, ret, c);
  }

  // Seek begin

  seek_ret = lseek(fd, 0, SEEK_SET);
  printf("lseek(fd, 0, SEEK_SET) returned %ld\n", seek_ret);
  if (seek_ret < 0) {
    perror("lseek(fd, 0, SEEK_SET)");
  }

  for (int i = 0; i < 3; ++i) {
    ret = read(fd, &c, 1);
    printf("read(%d, _, 1) returned %ld, c = '%c'\n", fd, ret, c);
  }

  // Seek end

  seek_ret = lseek(fd, 0, SEEK_END);
  printf("lseek(fd, 0, SEEK_END) returned %ld\n", seek_ret);
  if (seek_ret < 0) {
    perror("lseek(fd, 0, SEEK_END)");
  }

  for (int i = 0; i < 3; ++i) {
    ret = read(fd, &c, 1);
    printf("read(%d, _, 1) returned %ld, c = '%c'\n", fd, ret, c);
  }

  // Seek mid

  seek_ret = lseek(fd, 3, SEEK_SET);
  printf("lseek(fd, 3, SEEK_SET) returned %ld\n", seek_ret);
  if (seek_ret < 0) {
    perror("lseek(fd, 3, SEEK_SET)");
  }

  for (int i = 0; i < 3; ++i) {
    ret = read(fd, &c, 1);
    printf("read(%d, _, 1) returned %ld, c = '%c'\n", fd, ret, c);
  }

  // Seek curr

  seek_ret = lseek(fd, -2, SEEK_CUR);
  printf("lseek(fd, -2, SEEK_CUR) returned %ld\n", seek_ret);
  if (seek_ret < 0) {
    perror("lseek(fd, -2, SEEK_CUR)");
  }

  for (int i = 0; i < 3; ++i) {
    ret = read(fd, &c, 1);
    printf("read(%d, _, 1) returned %ld, c = '%c'\n", fd, ret, c);
  }


  close(fd);
  return 0;
}
