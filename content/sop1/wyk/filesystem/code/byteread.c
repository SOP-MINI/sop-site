#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {
  printf("pid = %d\n", getpid());

  if (argc < 2)
    return 1;

  int fd = open(argv[1], O_RDONLY);
  printf("fd = %d\n", fd);

  if (fd < 0)
    return 2;

  char c = '?';
  do {
    printf("%c> ", c);
    fflush(stdout);
    getchar();
  } while(read(fd, &c, 1) == 1);

  close(fd);

  return 0;
}
