---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

### Base race condition

```shell
make race
./race
```
Source: [race.c]({{< github_url "race.c" >}})

### Naive Peterson implementation

```shell
make peterson_naive
./peterson_naive
```
Source: [peterson_naive.c]({{< github_url "peterson_naive.c" >}})

### Reorderings

```shell
make store_buffer_test
./store_buffer_test
```
Source: [store_buffer_test.c]({{< github_url "store_buffer_test.c" >}})

Try inserting `atomic_thread_fence()` and observe reorderings gone!


### Peterson correct

```shell
make peterson_atomic
./peterson_atomic
```
Source: [peterson_atomic.c]({{< github_url "peterson_atomic.c" >}})

Note that using `atomic_int` solves all problems:
- provides atomic store-load behavior
- prevents any kinds of reorderings around stores and loads

Compare timings:

```shell
make all
time ./race
time ./peterson_naive
time ./peterson_atomic
```

### Atomic swap

```shell
make atomic_swap
./atomic_swap
```
Source: [atomic_swap.c]({{< github_url "atomic_swap.c" >}})

### Atomic Test-And-Set

```shell
make atomic_tas
./atomic_tas
```
Source: [atomic_tas.c]({{< github_url "atomic_tas.c" >}})

Try blocking inside CS and see what happens with CPU:

```shell
make atomic_tas_pause
./atomic_tas_pause
```

```shell
top -H -p $(pidof atomic_tas_pause)
```

### Semaphore

```shell
make semaphore
./semaphore
```
Source: [semaphore.c]({{< github_url "semaphore.c" >}})

```shell
make semaphore_pause
./semaphore_pause
```

```shell
top -H -p $(pidof semaphore_pause)
```

### Futex mutex

```shell
make futex
strace ./futex
```
