/* read_dup.c - the program demonstrates, that file descriptor, which
 * is obtained by dup() call is related to the same file sessions
 * that the original descriptor. In effect operations on each of
 * file descriptors affect common "current position in the file" counter.
 */

#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	char c;
	int fd1, fd2;
	struct timespec ts = { 0, 50000000 };

	if ((fd1 = open("read_dup.c", O_RDONLY)) < 0) {
		perror("open");
		return 1;
	}
	fd2 = dup(fd1);
	while (1) {
		if (read(fd1, &c, 1) < 1)
			return 0;
		if (write(1, &c, 1) != 1) {
			perror("write 1");
			return 2;
		}
		if (read(fd2, &c, 1) < 1)
			return 0;
		if (write(1, &c, 1) != 1) {
			perror("write 2");
			return 3;
		}
		nanosleep(&ts, NULL);
	}
	return 0;
}
