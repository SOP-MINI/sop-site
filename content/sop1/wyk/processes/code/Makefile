CC = gcc
CFLAGS = -Wall -g

TARGETS = vm cs_count hello fork_write wait orphan zombie env exec demon clone concurrent_read lab

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

cs_count: cs.c
	$(CC) $(CFLAGS) -O0 $< -o $@

clean:
	rm -f $(TARGETS) *.o
