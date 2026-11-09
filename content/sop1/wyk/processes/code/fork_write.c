#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, const char **argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: ./fork_write filename txt1 txt2\n");
    return 1;
  }

  int fd = open(argv[1], O_WRONLY | O_CREAT, 0666);
  if (fd < 0) {
    perror("open(): ");
    return 1;
  }

  const char *txt;

  switch (fork()) {
  case -1:
    perror("fork(): ");
    return 1;
  case 0:
    txt = argv[3];
    break;
  default:
    txt = argv[2];
    break;
  }

  // TODO: put open here, after fork

  while (1) {
    int ret = write(fd, txt, strlen(txt));
    if (ret < 0) {
      perror("write(): ");
      return 1;
    }
    sleep(1);
  }
}
