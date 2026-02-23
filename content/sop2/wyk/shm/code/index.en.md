---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

## Basic file mapping

Source: [basic.c]({{< github_url "basic.c" >}})

```shell
echo "I just love" > /tmp/poem.txt
echo "sharing memories" >> /tmp/poem.txt
echo "with other processes" >> /tmp/poem.txt
```

```shell
cat /tmp/poem.txt
```

```shell
gcc basic.c -o /tmp/basic
/tmp/basic /tmp/poem.txt
```

```shell
cat /tmp/poem.txt
```

Try experimenting:

- is `printf("%s")` safe?
- remove `PROT_WRITE` flag
- remove `PROT_READ` flag
- change to `MAP_PRIVATE`
- try to read past end of the mapping
- try to write past end of the mapping

## Binary file mapping

Source: [binary.c]({{< github_url "binary.c" >}})

Memory mapping is extremely powerful for binary files.
It allows you to treat the file contents directly as an array of C structures,
bypassing serialization, `read()`, and `write()` entirely.

```shell
gcc binary.c -o /tmp/binary
/tmp/binary /tmp/database.bin
```

```shell
cat /tmp/database.bin
```

```shell
xxd -c 40 /tmp/database.bin
```

## IPC through file mappings

Source: [fsipc.c]({{< github_url "fsipc.c" >}})

Two or more processes can map the exact same file using `MAP_SHARED`.
They will literally share the same physical memory pages.
This is the fastest form of Inter-Process Communication (IPC), but it needs **explicit synchronization**.

Initialize an empty file (or zero it out):

```shell
rm -f /tmp/counter.bin
```

Run single instance:

```shell
gcc fsipc.c -o /tmp/fsipc
/tmp/fsipc /tmp/counter.bin
```

Now run 3 instances in parallel:

```shell
rm -f /tmp/counter.bin
/tmp/fsipc /tmp/counter.bin &
/tmp/fsipc /tmp/counter.bin &
/tmp/fsipc /tmp/counter.bin &
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

Source: [procmaps.c]({{< github_url "procmaps.c" >}})

The Linux kernel exposes the internal memory layout of every running process via the `procfs` virtual filesystem. By
inspecting `/proc/<pid>/maps`, we can see exactly where the stack, heap, code, and our `mmap()` regions reside in the
virtual address space.

```shell
gcc procmaps.c -o /tmp/procmaps
/tmp/procmaps procmaps.c
```

```shell
cat /proc/$(pidof procmaps)/maps
```

Note how different parts of binary program are just mapped into the memory.

### Anonymous shared mappings

Source: [anon.c]({{< github_url "anon.c" >}})

In this example, the parent and child share an anonymous memory mapping. The child blocks and waits for a specific
signal (`sigwait`) before attempting to read the memory. The parent simulates some work, writes the data to the memory,
and then sends `SIGUSR1` to notify the child.

```shell
gcc anon.c -o /tmp/anon
/tmp/anon procmaps.c
```

Experiment:
- change to `MAP_PRIVATE`

### POSIX shared memory

Source: [shm_basic.c]({{< github_url "shm_basic.c" >}})

```shell
gcc shm_basic.c -o /tmp/shm_basic
/tmp/shm_basic arbiter
```

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

Source: [shm_tug.c]({{< github_url "shm_tug.c" >}})

```shell
gcc shm_tug.c -o /tmp/shm_tug
/tmp/shm_tug arbiter
```

Run in separate terminal:

```shell
/tmp/shm_tug left & /tmp/shm_tug right &
wait
```

Note this program is invalid due to lack of synchronization.

Try experiment:
- compile with optimizations (`-O2`)