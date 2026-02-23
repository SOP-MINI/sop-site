CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lrt

TARGETS = anon basic binary fsipc procmaps shm_basic shm_tug

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o data.bin