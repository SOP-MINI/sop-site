#define _GNU_SOURCE
#include <aio.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define BLOCKS 3
#define SHIFT(counter, x) ((counter + x) % BLOCKS)

void error(char *);
void usage(char *);
void siginthandler(int);
void sethandler(void (*)(int), int);
off_t getfilelength(int);
void fillaiostructs(struct aiocb *, char **, int, int);
void suspend(struct aiocb *);
void readdata(struct aiocb *, off_t);
void writedata(struct aiocb *, off_t);
void syncdata(struct aiocb *);
void getindexes(int *, int);
void cleanup(char **, int);
void reversebuffer(char *, int);
void processblocks(struct aiocb *, char **, int, int, int);

volatile sig_atomic_t work;

void error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void usage(char *progname)
{
	fprintf(stderr, "%s workfile blocksize\n", progname);
	fprintf(stderr, "workfile - path to the file to work on\n");
	fprintf(stderr, "n - number of blocks\n");
	fprintf(stderr, "k - number of iterations\n");
	exit(EXIT_FAILURE);
}

void siginthandler(int sig)
{
	work = 0;
}

void sethandler(void (*f)(int), int sig)
{
	struct sigaction sa;
	memset(&sa, 0x00, sizeof(struct sigaction));
	sa.sa_handler = f;
	if (sigaction(sig, &sa, NULL) == -1)
		error("Error setting signal handler");
}

off_t getfilelength(int fd)
{
	struct stat buf;
	if (fstat(fd, &buf) == -1)
		error("Cannot fstat file");
	return buf.st_size;
}

void suspend(struct aiocb *aiocbs)
{
	struct aiocb *aiolist[1];
	aiolist[0] = aiocbs;
	if (!work)
		return;
	while (aio_suspend((const struct aiocb *const *)aiolist, 1, NULL) == -1) {
		if (!work)
			return;
		if (errno == EINTR)
			continue;
		error("Suspend error");
	}
	if (aio_error(aiocbs) != 0)
		error("Suspend error");
	if (aio_return(aiocbs) == -1)
		error("Return error");
}

void fillaiostructs(struct aiocb *aiocbs, char **buffer, int fd, int blocksize)
{
	int i;
	for (i = 0; i < BLOCKS; i++) {
		memset(&aiocbs[i], 0, sizeof(struct aiocb));
		aiocbs[i].aio_fildes = fd;
		aiocbs[i].aio_offset = 0;
		aiocbs[i].aio_nbytes = blocksize;
		aiocbs[i].aio_buf = (void *)buffer[i];
		aiocbs[i].aio_sigevent.sigev_notify = SIGEV_NONE;
	}
}

void readdata(struct aiocb *aiocbs, off_t offset)
{
	if (!work)
		return;
	aiocbs->aio_offset = offset;
	if (aio_read(aiocbs) == -1)
		error("Cannot read");
}

void writedata(struct aiocb *aiocbs, off_t offset)
{
	if (!work)
		return;
	aiocbs->aio_offset = offset;
	if (aio_write(aiocbs) == -1)
		error("Cannot write");
}

void syncdata(struct aiocb *aiocbs)
{
	if (!work)
		return;
	suspend(aiocbs);
	if (aio_fsync(O_SYNC, aiocbs) == -1)
		error("Cannot sync\n");
	suspend(aiocbs);
}

void getindexes(int *indexes, int max)
{
	indexes[0] = rand() % max;
	indexes[1] = rand() % (max - 1);
	if (indexes[1] >= indexes[0])
		indexes[1]++;
}

void cleanup(char **buffers, int fd)
{
	int i;
	if (!work)
		if (aio_cancel(fd, NULL) == -1)
			error("Cannot cancel async. I/O operations");
	for (i = 0; i < BLOCKS; i++)
		free(buffers[i]);
	if (TEMP_FAILURE_RETRY(fsync(fd)) == -1)
		error("Error running fsync");
}

void reversebuffer(char *buffer, int blocksize)
{
	int k;
	char tmp;
	for (k = 0; work && k < blocksize / 2; k++) {
		tmp = buffer[k];
		buffer[k] = buffer[blocksize - k - 1];
		buffer[blocksize - k - 1] = tmp;
	}
}

void processblocks(struct aiocb *aiocbs, char **buffer, int bcount, int bsize, int iterations)
{
	int curpos, j, index[2];
	iterations--;
	curpos = iterations == 0 ? 1 : 0;
	readdata(&aiocbs[1], bsize * (rand() % bcount));
	suspend(&aiocbs[1]);
	for (j = 0; work && j < iterations; j++) {
		getindexes(index, bcount);
		if (j > 0)
			writedata(&aiocbs[curpos], index[0] * bsize);
		if (j < iterations - 1)
			readdata(&aiocbs[SHIFT(curpos, 2)], index[1] * bsize);
		reversebuffer(buffer[SHIFT(curpos, 1)], bsize);
		if (j > 0)
			syncdata(&aiocbs[curpos]);
		if (j < iterations - 1)
			suspend(&aiocbs[SHIFT(curpos, 2)]);
		curpos = SHIFT(curpos, 1);
	}
	if (iterations == 0)
		reversebuffer(buffer[curpos], bsize);
	writedata(&aiocbs[curpos], bsize * (rand() % bcount));
	suspend(&aiocbs[curpos]);
}

int main(int argc, char *argv[])
{
	char *filename, *buffer[BLOCKS];
	int fd, n, k, blocksize, i;
	struct aiocb aiocbs[4];
	if (argc != 4)
		usage(argv[0]);
	filename = argv[1];
	n = atoi(argv[2]);
	k = atoi(argv[3]);
	if (n < 2 || k < 1)
		return EXIT_SUCCESS;
	work = 1;
	sethandler(siginthandler, SIGINT);
	if ((fd = TEMP_FAILURE_RETRY(open(filename, O_RDWR))) == -1)
		error("Cannot open file");
	blocksize = (getfilelength(fd) - 1) / n;
	fprintf(stderr, "Blocksize: %d\n", blocksize);
	if (blocksize > 0) {
		for (i = 0; i < BLOCKS; i++)
			if ((buffer[i] = (char *)calloc(blocksize, sizeof(char))) == NULL)
				error("Cannot allocate memory");
		fillaiostructs(aiocbs, buffer, fd, blocksize);
		srand(time(NULL));
		processblocks(aiocbs, buffer, n, blocksize, k);
		cleanup(buffer, fd);
	}
	if (TEMP_FAILURE_RETRY(close(fd)) == -1)
		error("Cannot close file");
	return EXIT_SUCCESS;
}
