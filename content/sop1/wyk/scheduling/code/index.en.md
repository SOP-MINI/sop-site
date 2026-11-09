---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

### Prerequisites

Install `perf` and `hotspot`.

```shell
# Configure kernel parameters to allow tracing.
sudo sysctl -w kernel.perf_event_paranoid=1
sudo sysctl -w kernel.kptr_restrict=0
```

### Visualize CPU/IO phases with perf+hotspot

```shell
# Build example application
#gcc -g -O0 -o bursts bursts.c
make bursts
```
Source: [bursts.c]({{< github_url "bursts.c" >}})

```bash
# Capture performance snapshot to perf.data
sudo perf record -g --switch-events --sample-cpu -e "sched:sched_switch" -e "cycles" ./bursts &&
sudo chown $USER:$USER perf.data
```

* `-e cycles`: record PMU cycle counter expiration (event generated at fixed intervals within execution time)
* `-e sched:sched_switch`: record scheduler switch events - when CPU is given to some other task

```shell
# Visualize performance snapshot with `hotspot` GUI
hotspot perf.data
```

### CPU pinning

```shell
# Allow to run on CPU#2 only
#gcc -g -O0 -DCPU_AFFINITY=2 -o bursts_pinned bursts.c
make bursts_pinned
```

### Threads

```shell
# Build example threading application
#gcc -g -O0 -o threads threads.c -pthread
make threads
./threads
```
Source: [threads.c]({{< github_url "threads.c" >}})

```bash
sudo perf record -g --switch-events --sample-cpu -e "sched:sched_switch" -e "cycles" ./threads &&
sudo chown $USER:$USER perf.data &&
hotspot perf.data
```

```shell
# Allow to run on CPU#3 only
#gcc -g -O0 -DCPU_AFFINITY=3 -o threads_pinned threads.c -pthread
make threads_pinned
```

### Process scheduling attributes

```shell
# List processes with scheduling information
ps -c ax
```

```text
-   not reported
TS  SCHED_OTHER
FF  SCHED_FIFO
RR  SCHED_RR
B   SCHED_BATCH
ISO SCHED_ISO
IDL SCHED_IDLE
DLN SCHED_DEADLINE
?   unknown value
```

```shell
# Build example threading application
#gcc -g -O0 -o policy policy.c -pthread
make policy
./policy
```
Source: [policy.c]({{< github_url "policy.c" >}})

```shell
sudo ./policy
```

```bash
sudo perf record -g --switch-events --sample-cpu -e "sched:sched_switch" -e "cycles" ./policy &&
sudo chown $USER:$USER perf.data &&
hotspot perf.data
```

```bash
# Show threads while running the above capture
ps -p $(pidof policy) -c am
```
