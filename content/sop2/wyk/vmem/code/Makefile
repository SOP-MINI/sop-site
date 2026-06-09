CC = gcc
CFLAGS = -Wall -Wextra

TARGETS = large_mmap pf_cost alloc

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

pf_cost: CFLAGS += -O3

clean:
	rm -f $(TARGETS) *.o