CC = gcc
CFLAGS = -Wall -g -O0 -pthread

TARGETS = bursts bursts_pinned threads threads_pinned policy

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

bursts_pinned: bursts.c
	$(CC) $(CFLAGS) -DCPU_AFFINITY=2 $< -o $@

threads_pinned: threads.c
	$(CC) $(CFLAGS) -DCPU_AFFINITY=3 $< -o $@

clean:
	rm -f $(TARGETS) *.o perf.data perf.data.old
