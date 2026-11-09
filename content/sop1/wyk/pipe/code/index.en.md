---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

### Creating an anonymous pipe

```shell
make 01_pipe_creation
./01_pipe_creation
```
Source: [01_pipe_creation.c]({{< github_url "01_pipe_creation.c" >}})

### Parent-to-child communication

```shell
make 02_pipe_to_child
./02_pipe_to_child
```
Source: [02_pipe_to_child.c]({{< github_url "02_pipe_to_child.c" >}})

### Child-to-parent communication

```shell
make 03_pipe_to_parent
./03_pipe_to_parent
```
Source: [03_pipe_to_parent.c]({{< github_url "03_pipe_to_parent.c" >}})

### Wrapping pipe descriptors in C streams (`fdopen`)

```shell
make 04_pipe_stream_to_parent
./04_pipe_stream_to_parent
```
Source: [04_pipe_stream_to_parent.c]({{< github_url "04_pipe_stream_to_parent.c" >}})

### Writing to a pipe with closed read-end (`SIGPIPE`)

```shell
make 05_epipe
./05_epipe
echo $?
```
Source: [05_epipe.c]({{< github_url "05_epipe.c" >}})

### Handling `SIGPIPE` (`EPIPE` error)

```shell
make 06_epipe_with_handler
./06_epipe_with_handler
```
Source: [06_epipe_with_handler.c]({{< github_url "06_epipe_with_handler.c" >}})

### Detecting EOF when write-end is closed

```shell
make 07_read_close
./07_read_close
```
Source: [07_read_close.c]({{< github_url "07_read_close.c" >}})

Try experimenting:
- comment out `close(fd[1])` in the child or parent process and observe that `read()` blocks instead of returning `0`

### Stream nature of pipes (message sticking)

```shell
make 08_write_sticking
./08_write_sticking
```
Source: [08_write_sticking.c]({{< github_url "08_write_sticking.c" >}})

### Pipe buffer capacity

```shell
make 09_pipe_capacity
./09_pipe_capacity
```
Source: [09_pipe_capacity.c]({{< github_url "09_pipe_capacity.c" >}})

### Atomic writes and `PIPE_BUF`

```shell
make 10_pipebuf
./10_pipebuf
```
Source: [10_pipebuf.c]({{< github_url "10_pipebuf.c" >}})

Try experimenting:
- change the final write size from `PIPE_BUF` to `PIPE_BUF + 1` and observe a partial write instead of `EAGAIN`

### Creating and opening a FIFO (named pipe)

```shell
make 11_fifo_creation
./11_fifo_creation
```
Source: [11_fifo_creation.c]({{< github_url "11_fifo_creation.c" >}})

In a second terminal:

```shell
ls -l my.fifo
echo "Hello FIFO!" > my.fifo
```

Try experimenting:
- open the FIFO with `O_WRONLY` and read from it using `cat my.fifo` in another terminal
- add the `O_NONBLOCK` flag to `open()`

### Communication through a FIFO

```shell
make 12_fifo_communication
./12_fifo_communication
```
Source: [12_fifo_communication.c]({{< github_url "12_fifo_communication.c" >}})

### FIFO opening deadlock

```shell
make 13_fifo_deadlock
./13_fifo_deadlock
```
Source: [13_fifo_deadlock.c]({{< github_url "13_fifo_deadlock.c" >}})
