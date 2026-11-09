#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

void print_stat(const struct stat *s) {
  dev_t dev = s->st_dev;             /* ID of device containing file */
  ino_t ino = s->st_ino;             /* Inode number */
  nlink_t nlink = s->st_nlink;       /* File type and mode */
  mode_t mode = s->st_mode;          /* Number of hard links */
  uid_t uid = s->st_uid;             /* User ID of owner */
  gid_t gid = s->st_gid;             /* Group ID of owner */
  dev_t rdev = s->st_rdev;           /* Device ID (if special file) */
  off_t size = s->st_size;           /* Total size, in bytes */
  blkcnt_t blksize = s->st_blksize;  /* Block size for filesystem I/O */
  blksize_t blocks = s->st_blocks;   /* Number of 512B blocks allocated */
  struct timespec atim = s->st_atim; /* Time of last access */
  struct timespec mtim = s->st_mtim; /* Time of last modification */
  struct timespec ctim = s->st_ctim; /* Time of last status change */

  printf("dev = %lu\n", dev);
  printf("ino = %lu\n", ino);
  printf("nlink = %lu\n", nlink);
  printf("mode = %08x\n", mode);
  printf("uid = %d\n", uid);
  printf("gid = %d\n", gid);
  printf("rdev = %lu\n", rdev);
  printf("size = %ld\n", size);
  printf("blksize = %ld\n", blksize);
  printf("blocks = %ld\n", blocks);
  printf("atim = %ld.%.9ld\n", atim.tv_sec, atim.tv_nsec);
  printf("mtim = %ld.%.9ld\n", mtim.tv_sec, mtim.tv_nsec);
  printf("ctim = %ld.%.9ld\n", ctim.tv_sec, ctim.tv_nsec);
}

int main(int argc, const char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Filename required!\n");
    return 1;
  }
  const char *fn = argv[1];
  struct stat s;
  int ret;

  ret = stat(fn, &s);
  if (ret < 0) {
    perror("stat()");
  } else {
    print_stat(&s);
    printf("===========================\n");
  }

  ret = lstat(fn, &s);
  if (ret < 0) {
    perror("lstat()");
  } else {
    print_stat(&s);
    printf("===========================\n");
  }

  return 0;
}
