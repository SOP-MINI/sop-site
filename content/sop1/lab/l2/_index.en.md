---
title: "L2 - Low-Level File Operations and poll"
weight: 30
---

# Tutorial 1 - Filesystem

## Low-Level File Operations

You can also use low-level functions to read and write files-functions provided directly by the operating system rather than by the C standard library.
With these functions, you can, for instance, send packets over a network, which we’ll cover in the next semester.

Instead of using `FILE` pointers, low-level functions work with *file descriptors* (`fd`)-integer values that identify resources within a process.
In this case, these will be files, but in general, they can refer to various system resources.
To use the functions below, include the headers `<fcntl.h>` and `<unistd.h>`.

```
int open(const char *path, int oflag, ...);
```

See `man 3p open`.

- `path` - path to the file being opened,
- `oflag` - flags for opening the file, combined with a bitwise OR `|`, similar to the mode used in `fopen` but also specifying behavior in various edge cases. See `man 3p open` for the full list. Important flags for the lab include:
    - an access mode, e.g. `O_RDONLY`, `O_WRONLY`, or `O_RDWR`
    - `O_CREAT` - create the file if it does not exist
    - `O_EXCL` - used with `O_CREAT`: create the file if it does not exist, or return an error if it already exists
    - `O_APPEND` - append data to the end of the file
    - `O_TRUNC` - truncate the file to zero length when opening it
    - `O_NONBLOCK` - open in nonblocking mode: `read` and `write` do not wait for data to become available or for space to write it
- `...` - a third argument may be needed when creating a file; it specifies the permissions.

This function returns a file descriptor `fd`, which we’ll use with the following functions.
A descriptor is a process resource, so it must be released with `close` (`man 3p close`) after use.

Low-level functions don’t use buffering.
`read` (see `man 3p read`) receives characters as soon as they’re available.
This means it doesn’t always read as many characters as we expect.
On one hand, we get data as quickly as possible without waiting for the system to load the rest from the disk.
On the other hand, we need to make sure we actually read all the data we want.

```
ssize_t read(int fildes, void *buf, size_t nbyte);
```

- `fildes` - file descriptor obtained from `open`,
- `buf` - pointer to the buffer where data will be stored,
- `nbyte` - size of the data the function is expected to read.

This function returns the number of bytes successfully read.

The `write` function works similarly (see `man 3p write`):

```
ssize_t write(int fildes, const void *buf, size_t nbyte);
```

- `fildes` - file descriptor obtained from `open`,
- `buf` - pointer to the buffer containing the data,
- `nbyte` - size of the data to be sent.

The return value indicates the number of bytes successfully written.

These functions read or write data sequentially.
Sometimes, however, we need to access data somewhere in the middle of a file.
We could read enough bytes to reach it, but that would be inefficient.
The `lseek` function (`man 3p lseek`) lets us move the file position directly:

```
off_t lseek(int fildes, off_t offset, int whence);
```

The `whence` parameter can be `SEEK_SET` (set the position relative to the beginning of the file), `SEEK_CUR` (relative to the current position), or `SEEK_END` (relative to the end of the file).
Since `lseek` returns the position after the operation, calling it with `0` and `SEEK_CUR` gives the current position in the file.

When browsing the manuals, you may have noticed counterparts of familiar functions with an `-at` suffix, such as `fstatat`, `openat`, and `symlinkat`.
These functions usually take an additional parameter: a file descriptor relative to which paths are resolved.
They can make programming more convenient by avoiding the need to construct long relative paths or change the working directory.

### Task

Write a simple program that copies files.  
It should accept two paths as arguments and copy the file from the first path to the second.  

This time, use low-level functions.

### Task Solution

What the student needs to know:
- `man 3p open`
- `man 3p close`
- `man 3p read`
- `man 3p write`
- `man 3p mknod` (only constants describing permissions for `open`)

**Code for file** `l2-1.c`
{{< includecode "l2-1.c" >}}

### Notes and Questions

- Could this program be implemented using the standard C library functions instead of low-level I/O (e.g., `fopen`, `fprintf`, etc.)?  
{{< answer >}}  
Yes, there’s nothing in this program that prevents us from using the functions shown earlier.
{{</ answer >}}

- Can we write data to a file descriptor returned by `open` using `fprintf`?  
{{< answer >}}  
No! Functions like `fprintf`, `fgets`, and `fscanf` accept a `FILE*` variable as an argument, while a file descriptor is an `int` used by the operating system to identify an open file.
{{</ answer >}}

## Vectorized File Operations

The `writev` function (see `man 3p writev`) provides a convenient solution when the data we want to write isn’t in a single continuous memory fragment. It allows us to gather data from multiple locations and write it to a file with a single function call. It’s defined in `<sys/uio.h>`.

```
ssize_t writev(int fildes, const struct iovec *iov, int iovcnt);
```

- `fildes` - descriptor to which data is written,
- `iov` - array of structures describing the buffers from which data is gathered - `struct iovec` (see `man 0p sys_uio.h`), which has the following fields: 
```
void   *iov_base -> pointer to the memory area
size_t  iov_len  -> length of the memory area
```
- `iovcnt` - size of the `iov` array.

This function writes to `fildes`:\
`iov[0].iov_len` bytes starting from `iov[0].iov_base`,\
`iov[1].iov_len` bytes starting from `iov[1].iov_base`, … up to\
`iov[iovcnt-1].iov_len` bytes starting from `iov[iovcnt-1].iov_base`.

The `readv` function works similarly (see `man 3p readv`):

```
ssize_t readv(int fildes, const struct iovec *iov, int iovcnt);
```

- `fildes` - descriptor from which data is read,
- `iov` - array of `struct iovec` structures describing the buffers into which data is scattered,
- `iovcnt` - size of the `iov` array.

In other aspects, these functions work analogously to their non-vector counterparts `write` and `read`.

### Useful pages

- man 3p writev
- man 3p readv
- man 0p sys_uio.h


## Mmap

Another very convenient way to access a file is to map it into memory.
The `mmap` function does this.
It is also widely used for many other purposes, which we will discuss later.
For now, we will focus only on mapping files.
It usually looks like this:

{{< includecode "l2-2.c" >}}

The most important function here is, of course, `mmap` itself:

```
char *buff = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
```

Here we ask the operating system to map the file identified by `fd` into a memory region of size `length`, so that we can both read and modify it (`PROT_READ | PROT_WRITE`).
The result is a pointer to the mapped memory, or `MAP_FAILED` if an error occurs.
You can read about the other parameters and uses of this function in the manual (`man 3p mmap`, or `man 2 mmap` for Linux extensions), but they will not be required in the lab.

For now, the important point is that, as the code above shows, `mmap` lets us modify files conveniently, just like ordinary arrays.
Two helper functions are `msync` and `munmap`.
`msync` requests synchronization of changes to disk; the operating system does not always do this immediately because disk writes are slow.
`munmap` removes the mapping as part of cleanup.

One important point is that the descriptor must have the appropriate access mode.
If we passed `PROT_READ | PROT_WRITE` to `mmap` together with a descriptor opened with `O_RDONLY`, for example, the function would return an error.

## Blocking and Nonblocking Operations and fcntl

In the description of `open`, we mentioned the `O_NONBLOCK` flag, which lets us open a file in *nonblocking* mode.
By default, files are opened in blocking mode (remember that we are working with POSIX systems, where “everything is a file”).
A call to `read` or `write` on such a descriptor may wait until data is available to read or space is available to write.

The terminal and functions such as `scanf` and `getchar` are examples of this behavior.
When we call them, program execution stops until we provide some input on the terminal.
If we switch `stdin` to nonblocking mode, however, operations on it return immediately; consider the following example:

{{< includecode "l2-3.c" >}}

Compile and test the program above.
In the first, blocking part, both `scanf` and `read` wait for us to enter data and press Enter (by default, the terminal does not send input until it is confirmed with Enter).
After switching the descriptor to nonblocking mode, `read` behaves differently: it returns available data without waiting, or reports an error if no data is currently available.

This also shows that an error returned by a function (`-1` in this case) does not always mean the program must terminate immediately.
With a nonblocking `read`, this is expected: we check `errno` and find that the reason is simply that no data is available (`EAGAIN` or `EWOULDBLOCK`).
Only a different error tells us that something went wrong.

Also note the `fcntl` function (`man 3p fcntl`).
It lets us inspect and change a descriptor's properties (flags) after opening it:

```
int fcntl(int fildes, int cmd, ...);
```

Both `cmd` and the optional third argument can take many values.
For file status flags, the commands we need are `F_GETFL` and `F_SETFL`.
`F_GETFL` returns the current flags as a bit mask in an `int`.
`F_SETFL` takes a mask specifying the flags we want to set.
That is why the usual process looks like the `set_nonblocking()` function in the code above: first get the current flags, set the bits we need, and then write them back.
Calling `fcntl(fd, F_SETFL, O_NONBLOCK)` instead would clear other settable file status flags.

Sometimes we need a file to behave differently depending on the context—for example, reading in blocking mode in one place and nonblocking mode in another.
Changing flags repeatedly would be inefficient.
Note that `dup()` does not solve this: both descriptors share the same open file description and its file status flags, including `O_NONBLOCK`.
To use different file status flags, open the file separately where supported.

## I/O Multiplexing

As the example above shows, nonblocking I/O is inconvenient when we are waiting for data to arrive.
We have to check periodically whether data is available; this is called *busy waiting* or *busy polling*.
If we write a loop that repeatedly checks for data while waiting, it will fully occupy a processor core.
Imagine if every open terminal worked this way: the computer would be unusable.

Alternatively, we can pause between checks, as in the example above with `sleep`.
This reduces the load on the system, but increases the program's response delay.
Note that busy waiting is not permitted in the lab unless the task explicitly allows it.

Blocking I/O might seem like the perfect solution: our program is suspended without consuming CPU time, and the system wakes it when data is available and it can continue.
But there is a basic problem: we can only wait on one descriptor at a time this way.
Until now this was not a problem, because we only knew about `stdin` (we will soon see some other examples).
But imagine having several descriptors like `stdin` on which we are waiting for data.
Nonblocking I/O with busy waiting might seem like the only solution, but it has the significant drawbacks described above.

To solve these problems and enable responsive handling of multiple descriptors, operating systems provide *I/O multiplexing*.
Several functions implement this mechanism: `select` and `poll` (POSIX), and `epoll` (Linux).
Below we describe `poll`.
You can find a description of `epoll` in the additional material.
It is a Linux extension rather than part of POSIX and is highly optimized on that system, but it will not be required in the lab.

### Poll

Let's start by looking at the signature of `poll` (`man 3p poll`):

```
int poll(struct pollfd fds[], nfds_t nfds, int timeout);
```

`fds` is an array of structures containing the descriptors we will wait for; we will look at it more closely in a moment.
`nfds` is the size of this array.
`poll` returns when a descriptor is ready for an operation or when `timeout` milliseconds have elapsed.
We can also pass `-1` to wait indefinitely.

The `pollfd` structure is described in `man 0p poll.h`.
It must define at least the following fields:

- `int fd` - the descriptor to wait for.
- `short events` - a bit mask of the events we want to wait for. The most important for us are `POLLIN`, which waits for data to be ready to read, and `POLLOUT`, which waits for the descriptor to be ready for writing.
- `short revents` - short for *returned events*: `poll` sets flags in this bit mask to indicate why it returned. These can include the events requested in `events` (`POLLIN` or `POLLOUT`), but also values such as `POLLERR`, `POLLHUP`, and `POLLNVAL`, which indicate an error or a problem with the descriptor. See the manual for details.

To use `poll`, we create an `fds` array and put the descriptors and events we want to wait for in it.
For example, to wait for a user to enter data on standard input, we set `.fd` to `0` or `STDIN_FILENO` and `.events` to `POLLIN`.
`poll` itself sets `revents`; it is safest to initialize it to zero.

In the following example, we try to read information from `/dev/input/event*` files.
On Linux, these provide data from keyboards, mice, and similar input devices.
Activating an input device—for example, pressing a key—causes a new `input_event` structure, defined in `linux/input.h`, to become available from the corresponding file.
More information about the Linux input subsystem is available at <https://www.kernel.org/doc/html/latest/input/input.html>.

The header `linux/input-event-codes.h` defines values used in the `input_event` structure—for example, the F key corresponds to the value 33.
You do not need to memorize these details; they are not required in the lab.
We include them as an interesting example.

{{< includecode "l2-4.c" >}}

Once the program is running, it prints incoming input events whenever you press a key, move the mouse, and so on.

If the program fails because of insufficient permissions, check the permissions of the files in `/dev/input`.
Depending on the system configuration, you may need to belong to the `input` group.
In that case, you can add your user to the `input` group (`usermod -aG input <username>`) or grant read permissions (`chmod a+r /dev/input/*`).
Either way, this requires administrator privileges (`sudo` or `su`).
Access to input devices can expose keystrokes, including passwords, so only grant it to trusted users.
You can also run the program with those privileges using `sudo`.

If you still cannot run the program (for example, because you are using a university computer without administrator privileges), don't worry; keep reading.
Analyze how the program works and try modifying it to work with the descriptors described below, which should not require special configuration.

When does the program finish?
{{< answer >}}
The program finishes after `poll` has returned 30 times.
This is not the same as receiving 30 events.
In one iteration we can read events from several devices, and each device may have several events queued.
{{< /answer >}}

Why did we open the files in nonblocking mode? Would the program work with descriptors in blocking mode?
{{< answer >}}
Be careful when combining blocking I/O with multiplexing.
Notice that `poll` only tells us that some data is available; we do not know how much.
With a nonblocking descriptor, we can simply keep reading until there is no more data, which is what we do.
With blocking descriptors, the program would wait in the read loop once it had consumed the available events.

We could remove the loop and read only one message at a time.
If several were available, `poll` would return immediately and we could read the next one.
However, we must be careful: here we know the data has the size of an `input_event` structure.
Without that guarantee, we might have to read one byte at a time.
Nonblocking I/O is much better suited to this case: blocking I/O has no advantages here and risks blocking the program.
{{< /answer >}}

Can we use `poll` to read ordinary files?
{{< answer >}}
We can, but there is not much point.
Ordinary files are always ready to read, so `poll` returns immediately.
{{< /answer >}}

What other kinds of files can we usefully poll?
Among those encountered so far (in the Operating Systems course and “UNIX System Basics”) are character devices, including terminals (`/dev/tty`, as well as our programs' standard input and output), the `/dev/input/*` devices shown above, and many other files in `/dev`.
Other examples include pipes and FIFOs, network sockets, and many other descriptors returned by system functions (`timerfd`, `signalfd`, `inotify`, etc.).

### Named Pipes in Brief

Named pipes, also known as FIFOs (`man 7 fifo`), are a special type of file through which programs can exchange data.
They will be covered in detail in Operating Systems 2 when discussing IPC (*inter-process communication*).
We will show the basics here because a FIFO is a good, simple example of a file that can be used with `poll`.

The easiest way to create a named pipe from the command line is `mkfifo <name>` (`man 1 mkfifo`).
This creates a FIFO file with the specified name.
Programs can write data to and read data from it, but the data is not retained: reading data from a FIFO also removes it.
The easiest way to try pipes is from the command line.
In one terminal, create a FIFO and start reading from it:

```
mkfifo my_fifo
cat my_fifo
```

`cat` will wait until data appears in the FIFO. In a second terminal, enter:

```
echo "message" > my_fifo
```

After this command runs, `cat` in the first terminal should print our message.
We can then remove the pipe using the ordinary `rm` command.

In a C program, we can create a FIFO using `mkfifo` (`man 3p mkfifo`):

```
int mkfifo(const char *path, mode_t mode);
```

`path` is the path and `mode` specifies the permissions, just as with `mkdir`.

Once the FIFO is created, we use it much like an ordinary file: open it with `open`, read and write with `read` and `write`, and finally close it with `close`.
We can also remove it with `unlink`.
Naturally, we can open it in blocking or nonblocking mode and use `poll` for multiplexing.

### Timerfd (Additional Material)

A simple and very useful mechanism for testing `poll` is `timerfd`.
It is a Linux extension (not part of POSIX), so it will not be required in the lab.
However, it is useful and interesting, so the rest of this subsection is worth reading.

`timerfd` lets us create timers that we then read through a descriptor.
The full description is in `man 2 timerfd`.

To create a new timer, use:

```
int timerfd_create(int clockid, int flags);
```

`clockid` specifies the system clock to use.
The simplest choice is `CLOCK_REALTIME`, but there are other options described in the manual.
Of the flags that can be set, `TFD_NONBLOCK` is particularly relevant to us.

Once the timer has been created, set it using:

```
int timerfd_settime(int fd, int flags, const struct itimerspec *new_value,
                         struct itimerspec *_Nullable old_value);
```

Pass the descriptor obtained from `timerfd_create`.
The `new_value` parameter specifies the timer settings.
The structure has two fields (`man 3type itimerspec`): `it_value` sets the time until the first expiration, and `it_interval` can be used to set periodic expirations.
Both fields are of type `timespec` (`man 3type timespec`), meaning they specify seconds and nanoseconds.
Many useful functions use `timespec`, so it is worth getting familiar with it.
The `old_value` parameter is optional and can be set to `NULL`; see the manual for more information.

{{< includecode "l2-5.c" >}}

The behavior of the program above should be clear.
To confirm that the timer works correctly, you can run the program with the `time` command, for example `time ./l2-5`.

Notice that we did not set the timer to nonblocking mode in this example.
The goal was to demonstrate this feature as simply as possible, so we use a single `read`.
As an exercise, try modifying the previous `poll` program so that it ends after a certain amount of time (or perhaps after a certain period of inactivity; try a few things!).

### Epoll (Additional Material)

`poll` is easy to use and solves most of our problems.
But what if there are a great many descriptors? When using `poll`, we still have to iterate over all of them, which limits performance.
On Linux, a possible solution is `epoll`, described in [separate additional material]({{< ref "/sop1/lab/l2/epoll" >}}).
You are encouraged to read it, although it will not be required in the lab.

## Source codes presented in this tutorial

{{% codeattachments %}}
