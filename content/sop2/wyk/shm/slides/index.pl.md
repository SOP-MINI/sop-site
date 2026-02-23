---
title: "Slides"
type: presentation
layout: single
---

# Shared Memory

Operating Systems 2 Lecture

<small>Warsaw University of Technology<br/>Faculty of Mathematics and Information Science</small>

---

### File access using syscalls

Traditional file operations (`read()`/`write()`) require copying data between the kernel buffer and the user buffer.

Each file access requires a dedicated system call.

```c
int fd = open("data.txt", O_RDONLY);
char user_buffer[128];
ssize_t ret = read(fd, user_buffer, sizeof(user_buffer));
if (ret < 0) { /* ... */ }
process(user_buffer);
ret = read(fd, user_buffer, sizeof(user_buffer));
if (ret < 0) { /* ... */ }
process(user_buffer);
```

---

### What is `mmap()`?

Note the following:

- POSIX file contents is a sequence of bytes
- so is the POSIX process memory (might not be contiguous)
- it is therefore feasible to establish a mapping between the two
    - treat the file contents **as if it was** memory

**`mmap()`** maps a file (or other OS object) directly into the address space of the process
so that process can access file contents through memory operations.

---

### Memory file mappings

![mmap.svg](/wyk/mmap.svg)

---

### `mmap()` Syntax

```cpp
#include <sys/mman.h>

void* addr = mmap(
    NULL,                   // Address hint (null)
    length,                 // Length of the mapping
    PROT_READ | PROT_WRITE, // Memory protection flags
    MAP_SHARED,             // Flags (SHARED vs PRIVATE)
    fd,                     // File descriptor
    offset                  // Offset in the file
);

if (addr == MAP_FAILED) { /* handle error */ }
```

After the mapping is established `fd` is no longer needed.
After you're done use `munmap()` to release the mapping.

---

### Mapping visibility

| Flag                | Description                                                                                                              |
|:--------------------|:-------------------------------------------------------------------------------------------------------------------------|
| **`MAP_SHARED`**    | Modifications are visible to other processes mapping the same region and are **carried through to the underlying file**. |
| **`MAP_PRIVATE`**   | Modifications are private (Copy-on-Write). They are not written to the underlying file.                                  |
| **`MAP_ANONYMOUS`** | The mapping is not backed by any file; its contents are initialized to zero. The `fd` argument is ignored.               |

---

### Memory Protection

The `prot` argument describes the desired memory protection of the mapping. It must not conflict with the open mode of
the file.

| Flag             | Description                                     |
|:-----------------|:------------------------------------------------|
| **`PROT_READ`**  | Memory may be read.                             |
| **`PROT_WRITE`** | Memory may be written.                          |
| **`PROT_EXEC`**  | Memory may be executed (e.g., JIT compilation). |
| **`PROT_NONE`**  | Memory may not be accessed at all.              |

*Note: Flags can be combined using the bitwise OR operator (e.g., `PROT_READ | PROT_WRITE`).*

---

### IPC with memory-mapped files

Using `MAP_SHARED` processes may read and write the same **physical memory**
region backed by a disk file.

![ipc.svg](/wyk/ipc.svg)

---

### Private mappings

Each `MAP_PRIVATE` mapping gets own physical memory region. The sync process is disabled.

![private.svg](/wyk/private.svg)

<sub>Implementation detail: this is optimized to use Copy-on-Write scheme.</sub>

---

### Disk Synchronization

After mapping write the OS does not dump changes to the disk immediately.
Memory writes are asynchronously flushed to disk by kernel tasks.

![kworker.svg](/wyk/kworker.svg)

---

### Enforcing memory flushes

If application wishes to ensure I/O writes are done it may explicitly call `msync()`. 

```cpp
// Force changes to be written to the underlying file
if (msync(addr, length, MS_SYNC) == -1) {
    perror("msync");
}
```

This is necessary in case of software providing data durability (i.e. databases).

* **`MS_SYNC`**: Blocks the process until the data is physically written.
* **`MS_ASYNC`**: Schedules the write operation but returns immediately.

---

### The address space

Kernel represents address space of a process as an array of memory mappings, maintained inside the PCB.

Each mapping has associated a range of addresses (`start` + `length`).
It defines properties of given memory range:
- is it readable, writable, executable?
- shall changes be visible to other processes?
- is it backed by a file? if so - which one and which section of it?

`mmap()` is just a tool for a process to manage its own mappings (address space).

---

![procmaps.svg](/wyk/procmaps.svg)

---

### `procfs` debug interface

On Linux one may easily display contents of mappings array by simply reading from `procfs`.
Readable `/proc/<pid>/maps` contains line per-mapping:

```text
address       perms offset    dev   inode  pathname
400000-452000 r-xp  00000000 08:02 173521 /usr/bin/cat
651000-652000 r--p  00051000 08:02 173521 /usr/bin/cat
652000-655000 rw-p  00052000 08:02 173521 /usr/bin/cat
e03000-e24000 rw-p  00000000 00:00 0      [heap]
e24000-1f7000 rw-p  00000000 00:00 0      [heap]
...
```

`man 5 proc_pid_maps`

---

## Behavior on `fork()`

What happens to mappings when a process calls `fork()`?

* **Mappings are inherited**. The child receives a copy of the parent's page tables.
* **`MAP_SHARED`**: Child and parent see the *same* physical memory. A change by one is visible to the other.
* **`MAP_PRIVATE`**: A private copy of the region is created for the child process.

The child **will share memory** with parent if it had any shared mapping.

<sub>Implementation detail: The Copy-on-Write mechanism kicks in when inheriting private mappings.</sub>

---

![fork.svg](/wyk/fork.svg)

---

### Pure memory sharing

Applications just wanting to share memory are not interested in mapping filesystem objects.
For this purpose they can either:
- use a special kind of filesystem object: **POSIX shared memory object**
- use `MAP_ANONYMOUS`, `MAP_SHARED` mappings inherited during `fork()`

The former allows for sharing memory between unrelated processes.

---

### POSIX shared memory lifecycle

1. `shm_open()` - create/open the object
2. `ftruncate()` - set the size
3. `mmap()` - map into memory
4. `close()` - close the file descriptor (mapping remains)
5. `shm_unlink()` - remove the object name

---

### Creating the object

```cpp
#include <fcntl.h>
#include <sys/mman.h>

// The name must start with a slash "/"
int fd = shm_open("/my_data", O_CREAT | O_RDWR, 0666);

if (fd == -1) perror("shm_open");

// IMPORTANT: A new object has a size of 0!
// We must resize it:
if (ftruncate(fd, 1024) == -1) perror("ftruncate");
```

FD operations like `fchmod`, `fchown`, `fstat` work just like on regular files.

---

### SHM namespace

POSIX SHM objects are referred to through path-like strings but those have 
nothing to do with regular filesystem paths.

![shmnamespace.svg](/wyk/shmnamespace.svg)

SHM namespace is separate, has own root and is **flat**.

Linux provides debug view of this namespace through `/dev/shm/` directory.

---

## Mapping and usage

```cpp
int* shared_data = (int*)mmap(NULL, 1024, 
    PROT_READ | PROT_WRITE, 
    MAP_SHARED, // Must be SHARED for IPC
    fd, 0);

// fd no longer needed 
close(fd); 

// Using the memory
*shared_data = 42; 
```

Process B can now open the same shm object `/my_data` and read `42`.

---

## Cleanup (`shm_unlink`)

POSIX SHM objects have **kernel persistence**. They exist until the system reboots, or they are explicitly unlinked.

```cpp
// Removes the name from /dev/shm
shm_unlink("/my_data");
```

The object itself (memory) will be freed when the last process calls `munmap()`.

Check in the terminal:

```bash
$ ls -l /dev/shm
```

---

## Anonymous Mappings

If we only want to share memory between related processes (parent-child), we don't need a named object.

```cpp
void* addr = mmap(NULL, 4096, 
    PROT_READ | PROT_WRITE,
    MAP_SHARED | MAP_ANONYMOUS,
    -1, 0); // fd = -1
```

* Ideal for `fork()`.
* Memory is automatically freed when the processes terminate.
* Leaves no leftover garbage in `/dev/shm`.
