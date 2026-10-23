---
title: "L2 - Low-Level File Operations and poll"
date: 2022-02-07T19:36:16+01:00
weight: 30
---

# Tutorial 1 - Filesystem

## Low-Level File Operations

You can also use low-level functions to read and write files-functions provided directly by the operating system rather than by the C standard library. With these functions, you can, for instance, send packets over a network, which we’ll cover in the next semester.

Instead of using `FILE` pointers, low-level functions work with *file descriptors* (`fd`)-integer values that identify resources within a process. In this case, these will be files, but in general, they can refer to various system resources. To use the functions below, include the headers `<fcntl.h>` and `<unistd.h>`.

```
int open(const char *path, int oflag, ...);
```

See `man 3p open`.

- `path` - path to the file being opened,
- `oflag` - flags for opening the file, combined with a bitwise OR `|`, similar to the mode used in `fopen` but also specifying behavior in various edge cases. See `man 3p open` for the list of applicable flags.

This function returns a file descriptor `fd`, which we’ll use with the following functions.

Low-level functions don’t use buffering. `read` (see `man 3p read`) receives characters as soon as they’re available. This means it doesn’t always read as many characters as we expect. On one hand, we get data as quickly as possible without waiting for the system to load the rest from the disk. On the other hand, we need to make sure we actually read all the data we want.

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

**Code for file** `prog14.c`
{{< includecode "prog14.c" >}}

### Notes and Questions

- Why does the program above use the `bulk_read` and `bulk_write` functions?
Wouldn’t it be enough to just use `read` and `write`?  
{{< answer >}}  
According to the specification, `read` and `write` can return before the full amount of data requested by the user is read/written. You’ll learn more about this behavior in the next lab tutorial. In theory, this doesn’t matter for this task (since we’re not using signals), but it’s a good habit to develop now.
{{</ answer >}}

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
- `iov` - array of `struct iovec` structures describing the buffers

In other aspects, these functions work analogously to their non-vector counterparts `write` and `read`.

### Useful pages

- man 3p writev
- man 3p readv
- man 0p sys_uio.h


## Source codes presented in this tutorial

{{% codeattachments %}}
