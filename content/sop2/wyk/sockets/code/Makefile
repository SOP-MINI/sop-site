CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lrt

TARGETS = addrconv addrbind time_server time_client time_burst_client echo_server byte_sink simple_nc kv_store kv_client random_server random_client

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

time_server time_client time_burst_client: time_protocol.h

kv_store kv_client: kv_protocol.h

clean:
	rm -f $(TARGETS) *.o