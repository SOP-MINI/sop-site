CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lrt

TARGETS = race peterson_naive store_buffer_test peterson_atomic atomic_swap atomic_tas atomic_tas_pause semaphore semaphore_pause futex

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

atomic_tas_pause: atomic_tas.c
	$(CC) $(CFLAGS) -DPAUSE_IN_CS $< -o $@ $(LDFLAGS)

semaphore_pause: semaphore.c
	$(CC) $(CFLAGS) -DPAUSE_IN_CS $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o data.bin