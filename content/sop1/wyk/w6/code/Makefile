CC = gcc
CFLAGS = -Wall -g -pthread

TARGETS = basic_thread basic_thread_nojoin basic_thread_stack_version thread_args join detach \
          cancel cancel_nocancel cleanup cleanup_async concurrent_write concurrent_write_tsan \
          mutex thread_signals thread_kill thread_sigsuspend

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

basic_thread: basic_thread.c
	$(CC) $(CFLAGS) -fsanitize=address $< -o $@

basic_thread_nojoin: basic_thread.c
	$(CC) $(CFLAGS) -DNOJOIN $< -o $@

join: join.c
	$(CC) $(CFLAGS) -fsanitize=address $< -o $@

cancel_nocancel: cancel.c
	$(CC) $(CFLAGS) -DNOCANCEL $< -o $@

cleanup_async: cleanup.c
	$(CC) $(CFLAGS) -DASYNC $< -o $@

concurrent_write_tsan: concurrent_write.c
	$(CC) $(CFLAGS) -fsanitize=thread $< -o $@

clean:
	rm -f $(TARGETS) *.o
