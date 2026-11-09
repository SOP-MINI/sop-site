---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

### Creating a POSIX message queue

```shell
make 01_mqueue_creation
./01_mqueue_creation
```
Source: [01_mqueue_creation.c]({{< github_url "01_mqueue_creation.c" >}})

```shell
ls -l /dev/mqueue
```

### Default message queue attributes

```shell
make 02_default_attributes
./02_default_attributes
```
Source: [02_default_attributes.c]({{< github_url "02_default_attributes.c" >}})

```shell
cat /proc/sys/fs/mqueue/msg_default
cat /proc/sys/fs/mqueue/msgsize_default
```

### Custom queue attributes and system limits

```shell
make 03_create_attributes
./03_create_attributes
```
Source: [03_create_attributes.c]({{< github_url "03_create_attributes.c" >}})

```shell
cat /proc/sys/fs/mqueue/msg_max
cat /proc/sys/fs/mqueue/msgsize_max
```

### Message queue capacity (`O_NONBLOCK` vs blocking)

```shell
make 04_queue_capacity
./04_queue_capacity
```
Source: [04_queue_capacity.c]({{< github_url "04_queue_capacity.c" >}})

### Lack of EOF in message queues

```shell
make 05_no_eof
./05_no_eof
```
Source: [05_no_eof.c]({{< github_url "05_no_eof.c" >}})

### Timed receive (`mq_timedreceive`)

```shell
make 06_timeouts
./06_timeouts
```
Source: [06_timeouts.c]({{< github_url "06_timeouts.c" >}})

### Message priorities and message boundaries

```shell
make 07_priority
./07_priority
```
Source: [07_priority.c]({{< github_url "07_priority.c" >}})

### Asynchronous notification via signal (`SIGEV_SIGNAL`)

```shell
make 08_async_notification_signal
./08_async_notification_signal
```
Source: [08_async_notification_signal.c]({{< github_url "08_async_notification_signal.c" >}})

### Asynchronous notification via thread (`SIGEV_THREAD`)

```shell
make 09_async_notification_thread
./09_async_notification_thread
```
Source: [09_async_notification_thread.c]({{< github_url "09_async_notification_thread.c" >}})

### Memory-mapped file (`mmap`)

```shell
make 10_mmap
./10_mmap
cat file.txt
```
Source: [10_mmap.c]({{< github_url "10_mmap.c" >}}), [file.txt]({{< github_url "file.txt" >}})

### POSIX shared memory (`shm_open`, `mmap`)

```shell
make 11_shm
./11_shm
```
Source: [11_shm.c]({{< github_url "11_shm.c" >}})

```shell
ls -l /dev/shm
```

### System V message queues

```shell
make 12_sysv_msg
./12_sysv_msg
```
Source: [12_sysv_msg.c]({{< github_url "12_sysv_msg.c" >}})

```shell
ipcs -q
```

### System V shared memory

```shell
make 13_sysv_shm
./13_sysv_shm
```
Source: [13_sysv_shm.c]({{< github_url "13_sysv_shm.c" >}})

```shell
ipcs -m
```
