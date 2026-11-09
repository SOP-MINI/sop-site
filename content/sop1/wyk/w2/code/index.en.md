---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

## POSIX file permissions

```shell
echo "hello" > /tmp/hello.txt
stat /tmp/hello.txt
```

```shell
chmod 0700 /tmp/hello.txt
stat /tmp/hello.txt
```

## Linux file types

##### Regular files

```shell
echo "test" > /tmp/test.txt
stat /tmp/test.txt
```

```shell
cat /tmp/test.txt
```

```shell
echo "something" > /tmp/test.txt
```

##### Directories

```shell
mkdir /tmp/test.dir
stat /tmp/test.dir
```

```shell
cat /tmp/test.dir
```

```shell
strace cat /tmp/test.dir > /dev/null
```

```shell
echo "something" > /tmp/test.dir
```

##### Symbolic links

```shell
ln -sf /tmp/test.txt /tmp/test.txt.link
stat /tmp/test.txt.link
ll /tmp/test*
```

```shell
cat /tmp/test.txt.link
```

```shell
echo "linkwrite" > /tmp/test.txt.link
```

##### Character devices

```shell
ll /proc/$$/fd
```

```shell
stat /dev/pts/1
```

```shell
cat /dev/pts/1 # Run from a different terminal
```

```shell
echo "something" > /dev/pts/1 # Run from a different terminal
```

##### Block devices

```shell
lsblk
```

```shell
stat /dev/nvme0n1p1
```

```shell
sudo cat /dev/nvme0n1p1
```

```shell
sudo xxd -l512 /dev/nvme0n1p1
```

_No write testing as I like my disk structure :)_

##### Sockets

```shell
stat /run/docker.sock
```

```shell
cat /run/docker.sock
```

```shell
echo "something" > /run/docker.sock
```

##### FIFOs

```shell
mkfifo /tmp/test.fifo
stat /tmp/test.fifo
```

```shell
cat /tmp/test.fifo
```

```shell
echo "something" > /tmp/test.fifo # Run in different terminal
```

## Filesystem mounting

```shell
mount
```

```shell
df
```

## Linux kernel structures

Per-process pointer to descriptor table:
https://elixir.bootlin.com/linux/v6.11.3/source/include/linux/sched.h#L1121

Pointers to open files:
https://elixir.bootlin.com/linux/v6.11.3/source/include/linux/fdtable.h#L46

Open file structure:
https://elixir.bootlin.com/linux/v6.11.3/source/include/linux/fs.h#L996

I-node pointee:
https://elixir.bootlin.com/linux/v6.11.3/source/include/linux/fs.h#L1019

## Low-level syscalls

### open() syscall

```shell
make open
rm -f /tmp/no_such_file.txt
./open /tmp/no_such_file.txt
```
Source: [open.c]({{< github_url "open.c" >}})

```shell
echo "hello" > /tmp/there_is_such_file.txt
./open /tmp/there_is_such_file.txt
```

```shell
cat /tmp/there_is_such_file.txt
```

```shell
make append
echo "hello" > /tmp/append.txt
./append /tmp/append.txt
```
Source: [append.c]({{< github_url "append.c" >}})

Create file which doesn't exist:

```shell
make create_file
rm -f /tmp/create_file.txt
./create_file /tmp/create_file.txt
ls -l /tmp/create_file.txt
```
Source: [create_file.c]({{< github_url "create_file.c" >}})

_Try to remove last open() argument and see what happens_

Try to re-create with different perm:

```shell
#gcc -DMODE=0666 -o create_file_0666 -g create_file.c
make create_file_0666
./create_file_0666 /tmp/create_file.txt
ls -l /tmp/create_file.txt
```

Remove the file and re-run the creation:

```shell
rm -f /tmp/create_file.txt
./create_file_0666 /tmp/create_file.txt
ls -l /tmp/create_file.txt
```

```shell
umask
```

```shell
rm -f /tmp/create_file.txt
#gcc -DUMASK=0000 -DMODE=0666 -o create_file_umask -g create_file.c
make create_file_umask
./create_file_umask /tmp/create_file.txt
ls -l /tmp/create_file.txt
```

Exclusive open:

```shell
make excl
./excl /tmp/lock.txt
```
Source: [excl.c]({{< github_url "excl.c" >}})

Descriptor limit:

```shell
make descriptor_limit
./descriptor_limit
```
Source: [descriptor_limit.c]({{< github_url "descriptor_limit.c" >}})

```shell
ulimit -a
```

### read()/write() syscalls

Standard FDS:

```shell
#gcc -o standard_fds -g -fsanitize=address standard_fds.c
make standard_fds
./standard_fds
```
Source: [standard_fds.c]({{< github_url "standard_fds.c" >}})

File sequential access:

```shell
echo "test" > /tmp/test.txt
make byteread
./byteread /tmp/test.txt
```
Source: [byteread.c]({{< github_url "byteread.c" >}})

Observe contents of `/proc/<pid>/fd` and `/proc/<pid>/fdinfo` during runtime.

#### Read Without O_NONBLOCK

From short file:

```shell
echo -ne "x" > /tmp/short.txt
make read
./read /tmp/short.txt
```
Source: [read.c]({{< github_url "read.c" >}})

From longer file:

```shell
echo -ne "abcdefghijk" > /tmp/short.txt
./read /tmp/short.txt
```

From STDIN:

```shell
./read stdin
```

From FIFO:

From other terminal:
```shell
mkfifo /tmp/nonblock.fifo
cat > /tmp/nonblock.fifo
```

```shell
./read /tmp/nonblock.fifo
```

#### Read With O_NONBLOCK

From short file:

```shell
echo -ne "x" > /tmp/short.txt
#gcc -o read_nonblock -g read.c -DNONBLOCK
make read_nonblock
./read_nonblock /tmp/short.txt
```

From longer file:

```shell
echo -ne "abcdefghijk" > /tmp/short.txt
./read_nonblock /tmp/short.txt
```

From STDIN:

```shell
./read_nonblock stdin
```

From FIFO:

From other terminal:
```shell
mkfifo /tmp/nonblock.fifo
cat > /tmp/nonblock.fifo
```

```shell
./read_nonblock /tmp/nonblock.fifo
```

#### writev() syscall

```shell
make writev
./writev /tmp/writev.txt
```
Source: [writev.c]({{< github_url "writev.c" >}})

### stat() syscall

```shell
make stat
./stat /usr/bin/gcc
```
Source: [stat.c]({{< github_url "stat.c" >}})

### lseek() syscall

```shell
echo -ne "abcdefgh" > /tmp/lseek.txt
make lseek
./lseek /tmp/lseek.txt
```
Source: [lseek.c]({{< github_url "lseek.c" >}})

_Question: how to query current file position using lseek()?_

### Overlapping file descriptors

#### dup()

```shell
echo -ne > /tmp/dup.txt
make dup_write
./dup_write /tmp/dup.txt aaa bbb
```
Source: [dup_write.c]({{< github_url "dup_write.c" >}})

From other terminal:
```shell
watch -n0.1 cat /tmp/dup.txt
```

### Directory operations

```shell
make readdir
./readdir .
```
Source: [readdir.c]({{< github_url "readdir.c" >}})

```shell
strace ./readdir . > /dev/null
```

### Hardlinks

```shell
ln /tmp/test.txt /tmp/test2.txt
stat /tmp/test.txt
stat /tmp/test2.txt
```

```shell
cat /tmp/test2.txt
```

## C File streams

```shell
make fileno
./fileno
```
Source: [fileno.c]({{< github_url "fileno.c" >}})

```shell
make api_mixing
./api_mixing
```
Source: [api_mixing.c]({{< github_url "api_mixing.c" >}})

```shell
make fopen
./fopen
```
Source: [fopen.c]({{< github_url "fopen.c" >}})

```shell
make stream_limit
./stream_limit
```
Source: [stream_limit.c]({{< github_url "stream_limit.c" >}})

### Stream buffering

Default stdout buffering - line:

```shell
#gcc -DBUFF_MODE=_IOLBF -o stream_buffering_lbf -g stream_buffering.c
make stream_buffering_lbf
./stream_buffering_lbf
```
Source: [stream_buffering.c]({{< github_url "stream_buffering.c" >}})

```shell
strace --trace=write ./stream_buffering_lbf > /dev/null
```

Change to nonbuffered:

```shell
#gcc -DBUFF_MODE=_IONBF -o stream_buffering_nbf -g stream_buffering.c
make stream_buffering_nbf
./stream_buffering_nbf
```

```shell
strace --trace=write ./stream_buffering_nbf > /dev/null
```

Change to fully buffered:

```shell
#gcc -DBUFF_MODE=_IOFBF -o stream_buffering_fbf -g stream_buffering.c
make stream_buffering_fbf
./stream_buffering_fbf
```

```shell
strace --trace=write ./stream_buffering_fbf > /dev/null
```

### EOF testing

Read from stdin:

```shell
make eof
./eof
```
Source: [eof.c]({{< github_url "eof.c" >}})

Read from file:

```shell
echo "I" >> /tmp/eof.txt
echo "like" >> /tmp/eof.txt
echo "OPS" >> /tmp/eof.txt
./eof /tmp/eof.txt
```

What if we call scanf() on a stream with EOF indicator set but file has grown in the meantime?

### Input errors

```shell
make scanf_error
./scanf_error
```
Source: [scanf_error.c]({{< github_url "scanf_error.c" >}})

### Stream position

```shell
make ftell
./ftell
```
Source: [ftell.c]({{< github_url "ftell.c" >}})

```shell
./ftell /tmp/ftell.txt # Observe contents of /tmp/ftell.txt during run
```
