#ifndef DUMB_COMMON_H
#define DUMB_COMMON_H

#include <stdatomic.h>

#define PORT 8090

extern atomic_ullong active_threads;
extern atomic_ullong active_connections;
extern atomic_ullong total_ping_requests;
extern atomic_ullong total_wait_requests;
extern atomic_ullong total_burn_requests;
extern atomic_ullong total_stream_requests;
extern atomic_ullong total_unknown_requests;

void *metrics_server_thread(void *arg);

long long recursive_burn(long long depth);

void handle_client(int client_fd);

#endif
