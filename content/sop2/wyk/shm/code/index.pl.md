---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

### Basic file mapping

```shell
echo "I just love" > /tmp/poem.txt
echo "sharing memories" >> /tmp/poem.txt
echo "with other processes" >> /tmp/poem.txt
```

```shell
cat /tmp/poem.txt
```

```shell
make basic
./basic /tmp/poem.txt
```
Source: [basic.c]({{< github_url "basic.c" >}})

Try experimenting:

- is `printf("%s")` safe?
- remove `PROT_WRITE` flag
- remove `PROT_READ` flag
- change to `MAP_PRIVATE`
- try to read past end of the mapping
- try to write past end of the mapping

### Binary file mapping

Memory mapping is extremely powerful for binary files.
It allows you to treat the file contents directly as an array of C structures,
bypassing serialization, `read()`, and `write()` entirely.

```shell
rm /tmp/database.bin
make binary
./binary /tmp/database.bin
```
Source: [binary.c]({{< github_url "binary.c" >}})

```shell
cat /tmp/database.bin
```

```shell
xxd -c 40 /tmp/database.bin
```

### IPC through file mappings

Two or more processes can map the exact same file using `MAP_SHARED`.
They will literally share the same physical memory pages.
This is the fastest form of Inter-Process Communication (IPC), but it needs **explicit synchronization**.

Initialize an empty file (or zero it out):

Run single instance:

```shell
rm -f /tmp/counter.bin
make fsipc
./fsipc /tmp/counter.bin
```
Source: [fsipc.c]({{< github_url "fsipc.c" >}})

Now run 3 instances in parallel:

```shell
rm -f /tmp/counter.bin
./fsipc /tmp/counter.bin &
./fsipc /tmp/counter.bin &
./fsipc /tmp/counter.bin &
wait
```

Notice the final value. Is it 3B?

```shell
# print integer value
od -An -t d4 /tmp/counter.bin
```

Try experimenting:

- run 10 instances simultaneously in a bash loop. How low is the final value?
- change mapping to `MAP_PRIVATE`

### The address space

The Linux kernel exposes the internal memory layout of every running process via the `procfs` virtual filesystem. By
inspecting `/proc/<pid>/maps`, we can see exactly where the stack, heap, code, and our `mmap()` regions reside in the
virtual address space.

```shell
make procmaps
./procmaps procmaps.c
```
Source: [procmaps.c]({{< github_url "procmaps.c" >}})

```shell
cat /proc/$(pidof procmaps)/maps
```

Note how different parts of binary program are just mapped into the memory.

### Anonymous shared mappings

In this example, the parent and child share an anonymous memory mapping. The child blocks and waits for a specific
signal (`sigwait`) before attempting to read the memory. The parent simulates some work, writes the data to the memory,
and then sends `SIGUSR1` to notify the child.

```shell
make anon
./anon procmaps.c
```
Source: [anon.c]({{< github_url "anon.c" >}})

Experiment:
- change to `MAP_PRIVATE`

### POSIX shared memory


```shell
make shm_basic
./shm_basic arbiter
```
Source: [shm_basic.c]({{< github_url "shm_basic.c" >}})

```shell
ls /dev/shm
```

```shell
xxd /dev/shm/hello_shm
```

After first pause:

```shell
watch -n0.1 xxd /dev/shm/hello_shm
```

Try experiment:
- remove `shm_unlink()`
- play with `O_CREAT | O_EXCL`

### SHM Tug of war

```shell
make shm_tug
./shm_tug arbiter
```
Source: [shm_tug.c]({{< github_url "shm_tug.c" >}})

Run in separate terminal:

```shell
./shm_tug left & ./shm_tug right &
wait
```

Note this program is invalid due to lack of synchronization.

Try experiment:
- compile with optimizations (`-O2`)