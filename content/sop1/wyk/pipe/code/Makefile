CC = gcc
CFLAGS = -Wall -g -O0

TARGETS = \
	01_pipe_creation \
	02_pipe_to_child \
	03_pipe_to_parent \
	04_pipe_stream_to_parent \
	05_epipe \
	06_epipe_with_handler \
	07_read_close \
	08_write_sticking \
	09_pipe_capacity \
	10_pipebuf \
	11_fifo_creation \
	12_fifo_communication \
	13_fifo_deadlock

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS) *.o *.fifo
