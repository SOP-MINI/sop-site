CC = gcc
CFLAGS = -Wall -g

TARGETS = open append create_file create_file_0666 create_file_umask excl descriptor_limit \
          standard_fds byteread read read_nonblock writev stat lseek dup_write readdir \
          fileno api_mixing fopen stream_buffering stream_buffering_lbf stream_buffering_nbf stream_buffering_fbf \
          stream_limit eof scanf_error ftell

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

create_file_0666: create_file.c
	$(CC) $(CFLAGS) -DMODE=0666 $< -o $@

create_file_umask: create_file.c
	$(CC) $(CFLAGS) -DUMASK=0000 -DMODE=0666 $< -o $@

standard_fds: standard_fds.c
	$(CC) $(CFLAGS) -fsanitize=address $< -o $@

read_nonblock: read.c
	$(CC) $(CFLAGS) -DNONBLOCK $< -o $@

stream_buffering_lbf: stream_buffering.c
	$(CC) $(CFLAGS) -DBUFF_MODE=_IOLBF $< -o $@

stream_buffering_nbf: stream_buffering.c
	$(CC) $(CFLAGS) -DBUFF_MODE=_IONBF $< -o $@

stream_buffering_fbf: stream_buffering.c
	$(CC) $(CFLAGS) -DBUFF_MODE=_IOFBF $< -o $@

clean:
	rm -f $(TARGETS) *.o
