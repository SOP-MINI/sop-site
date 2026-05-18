---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

## Iterative DUMB server

We'll work with a TCP based service exposing a few text based commands:
* request `PING` -> response `PONG`
* request `WAIT <s>` -> blocks for `s` seconds, response `DONE`
* request `BURN <it>` -> runs `<it>` iterations of CPU intensive, dumb calculations, response `RESULT <x>`
* request `STREAM <n>` -> streams back `<n>` characters to client, then response `DONE`.

All delimited with `\n` character.

```shell
make dumb_iterative && ./dumb_iterative
```
[dumb_iterative.c]({{< github_url "dumb_iterative.c" >}})

Note how this simple iterative implementation makes the server not responsive
while it is processing some request.

Long-lasting requests cause other clients to wait:

```shell
echo "PING" | nc -vN 127.0.0.1 8090

echo "WAIT 5000" | nc -vN 127.0.0.1 8090 &
PID_WAIT=$!

time echo "PING" | nc -vN 127.0.0.1 8090

wait $PID_WAIT
```

Also, slow clients (slowly receiving data) delay handling of the others:

```shell
echo "STREAM 100000000" | nc -vN 127.0.0.1 8090 | pv -q -L 10 > /dev/null &
PID_SLOW=$!

sleep 1.0

echo "PING" | timeout 5 nc -vN 127.0.0.1 8090
if [ $? -eq 124 ]; then
    echo "[!] TIMEOUT: Client 2 gave up. The server is completely paralyzed."
fi

sleep 1.0

kill $PID_SLOW 2>/dev/null
wait $PID_SLOW 2>/dev/null
```

Same for a client who slowly sends requests:

```shell
(
    echo -n "P"; sleep 2;
    echo -n "I"; sleep 2;
    echo -n "N"; sleep 2;
    echo "G"
) | nc -vN 127.0.0.1 8090 &
PID_SLOWLORIS=$!

sleep 1.0

echo "PING" | timeout 5 nc -vN 127.0.0.1 8090
if [ $? -eq 124 ]; then
    echo "[!] TIMEOUT: Client 2 blocked by a slow sender!"
fi

wait $PID_SLOWLORIS 2>/dev/null
```

The server could easily handle others while **waiting for I/O**.

## Event multiplexers

Build and run the simple TCP chat server based on `select()`:

```shell
make select_chat && ./select_chat
```
[select_chat.c]({{< github_url "select_chat.c" >}})

Connect a few clients and send some messages:

```shell
nc localhost 8090
```

Now try `epoll()` version:

```shell
make epoll_chat && ./epoll_chat
```
[epoll_chat.c]({{< github_url "epoll_chat.c" >}})

## Thread-Per-Connection

Let's try a different approach to solve the above problems.
Server is now going to spawn a **dedicated thread** handling each connection. 

```shell
make dumb_multithreaded && taskset -c 0 ./dumb_multithreaded
```
[dumb_multithreaded.c]({{< github_url "dumb_multithreaded.c" >}})

Note how:
- `PING` request is handled in an instant while `WAIT 5000` is still running
- `PING` is simply handled while the other client slowly receives bytes from `STREAM` command
- The same in case the other client is slowly sending `P`, `I`, `N`, `G\n`, request bytes.

Another problem appears. Spin up a monitoring dashboard:

```shell
docker compose up -d
```

And view the dashboard at http://localhost:3000/.

Then run more and more clients issuing small `BURN` requests. Each python instance runs on separate core
and simulates 1k clients using the service.

```shell
taskset -c 1 python c10k_client.py
```

```shell
taskset -c 2 python c10k_client.py
```

```shell
taskset -c 3 python c10k_client.py
```

```shell
taskset -c 4 python c10k_client.py
```

```shell
taskset -c 5 python c10k_client.py
```

```shell
taskset -c 6 python c10k_client.py
```

```shell
taskset -c 7 python c10k_client.py
```

```shell
taskset -c 8 python c10k_client.py
```

```shell
taskset -c 9 python c10k_client.py
```

```shell
taskset -c 10 python c10k_client.py
```

Key observations:
- Number of threads = number of clients
- At some threshold (usually ~7k) the server becomes virtually unusable:
  - latency grows high
  - clients are not able to connect
  - requests do timeout
- CPU is utilized 100% at this time mostly by `%sys` (syscalls and context switches) 
- Server consumes HUGE amount of memory (proportional to the number of clients)
- Huge number of context switches, including non-voluntary appears

Experiment:
- change client request type (i.e. `PONG`/`STREAM`)
- try to limit server memory

## Solution with epoll

Now do the same against epoll implementation:

```shell
make dumb_epoll && taskset -c 0 ./dumb_epoll
```
[dumb_epoll.c]({{< github_url "dumb_epoll.c" >}})

Observe the load on the dashboards. Such a server can easily handle 10k clients, even with a single core!

Follow-ups: Try to implement `WAIT <n>` command handling.

## Scalable epoll

```shell
make dumb_epoll_workers && taskset -c 0,1,2,3 ./dumb_epoll_workers
```
[dumb_epoll_workers.c]({{< github_url "dumb_epoll_workers.c" >}})

Observe the load distribution on the dashboard at http://localhost:3000/dashboards: `DUMB epoll workers`

```shell
taskset -c 4 python c10k_client.py 2000
```

```shell
taskset -c 5 python c10k_client.py 2000
```

```shell
taskset -c 6 python c10k_client.py 2000
```

## Reactor pattern

```shell
make dumb_reactor && taskset -c 0,1,2,3 ./dumb_reactor
```
[dumb_reactor.c]({{< github_url "dumb_reactor.c" >}})

## Coroutines

```shell
make dumb_coro && taskset -c 0,1,2,3 ./dumb_coro
```
[dumb_coro.c]({{< github_url "dumb_reactor.c" >}})
