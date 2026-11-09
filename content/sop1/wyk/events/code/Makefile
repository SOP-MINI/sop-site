CC = gcc
CFLAGS = -Wall -g

TARGETS = async_fd timer_create timerfd_create signalfd watch_file watch_dir watch_tree uring_read select_basic select_multiple

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

timer_create: timer_create.c
	$(CC) $(CFLAGS) $< -o $@ -lrt

uring_read: uring_read.c
	$(CC) $(CFLAGS) $< -o $@ -luring

clean:
	rm -f $(TARGETS) *.o
