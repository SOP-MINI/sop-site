#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, const char **argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: ./dup_write filename txt1 txt2\n");
    return 1;
  }

  int fd1 = open(argv[1], O_WRONLY | O_CREAT, 0666);
  if (fd1 < 0) {
    perror("open(): ");
    return 1;
  }

  int fd2 = dup(fd1);

  while (1) {
    int ret = write(fd1, argv[2], strlen(argv[2]));
    if (ret < 0) {
      perror("write(): ");
      return 1;
    }
    getchar();

    ret = write(fd2, argv[3], strlen(argv[3]));
    if (ret < 0) {
      perror("write(): ");
      return 1;
    }
    getchar();
  }
}
