CC = gcc
CFLAGS = -Wall -g -O0
LDFLAGS = -lrt -pthread

TARGETS = \
	01_mqueue_creation \
	02_default_attributes \
	03_create_attributes \
	04_queue_capacity \
	05_no_eof \
	06_timeouts \
	07_priority \
	08_async_notification_signal \
	09_async_notification_thread \
	10_mmap \
	11_shm \
	12_sysv_msg \
	13_sysv_shm

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o
