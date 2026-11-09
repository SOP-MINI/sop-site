---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

### Async FD

```shell
make async_fd
./async_fd
```
Source: [async_fd.c]({{< github_url "async_fd.c" >}})

### Timers

```shell
make timer_create
./timer_create
```
Source: [timer_create.c]({{< github_url "timer_create.c" >}})

Try to:
* Use single shot timer
* Specify absolute time instead of relative one
* Create multiple parallel timers

```shell
make timerfd_create
./timerfd_create
```
Source: [timerfd_create.c]({{< github_url "timerfd_create.c" >}})

Try to:
* Use `TFD_NONBLOCK`
* _all from previous example_

### Signal descriptor

```shell
make signalfd
./signalfd
```
Source: [signalfd.c]({{< github_url "signalfd.c" >}})

Try to:
* C-c several times
* `kill -SIGTERM <pid>`
* Use `SFD_NONBLOCK`

### inotify

```shell
make watch_file
echo "hello" > /tmp/hello.txt
./watch_file /tmp/hello.txt
```
Source: [watch_file.c]({{< github_url "watch_file.c" >}})

Try to:
* modify file contents
* change inode attributes (`chmod`, `touch`)
* move file
* remove file itself

```shell
rm -Rf /tmp/testdir
mkdir /tmp/testdir
make watch_dir
./watch_dir /tmp/testdir
```
Source: [watch_dir.c]({{< github_url "watch_dir.c" >}})

Try to:
* add/remove file/subdir
* rename/move file
* remove watched directory

```shell
rm -Rf /tmp/tree
mkdir -p /tmp/tree/sub/subsub/
echo "main.txt" > /tmp/tree/main.txt
touch /tmp/tree/sub/.empty
echo "f1.txt" > /tmp/tree/sub/f1.txt
echo "f1.txt" > /tmp/tree/sub/f2.txt
touch /tmp/tree/sub/subsub/.empty
make watch_tree
./watch_tree /tmp/tree
```
Source: [watch_tree.c]({{< github_url "watch_tree.c" >}})

### io_uring

```shell
make uring_read
./uring_read /tmp
```
Source: [uring_read.c]({{< github_url "uring_read.c" >}})

### select

```shell
make select_basic
./select_basic /tmp
```
Source: [select_basic.c]({{< github_url "select_basic.c" >}})

```shell
make select_multiple
./select_multiple /tmp
```
Source: [select_multiple.c]({{< github_url "select_multiple.c" >}})
