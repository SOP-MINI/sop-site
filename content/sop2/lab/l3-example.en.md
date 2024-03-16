---
title: "Task on shared memory and mmap"
bookHidden: true
---

## Goal

Write two programs that communicate using POSIX shared memory: client and server.

The server takes the arguments `N` and `M`. It creates a shared memory object with the same name as the process id. The server sets the size of memory to `N`\*12 bytes. Then it displays the shared memory object's name to the terminal.

The server fills all `N` of the 12-byte memory chunks in a following way:

| Byte number | 0-4 | 4-8 | 8-12 |
| - | - | - | - |
| Contents | Random integer from 0 to `M` | Random integer from 0 to `M` | Zeroes (reserved for results) |

When the server receives `SIGINT` signal it writes the sum of the results (see below) to the terminal, deletes its shared memory and terminates.

The client program is called with 2 parameters - shared memory object's name and `K` - a number from 0 to `N`-1. The client opens the memory and reads the two integers from `K`\*12 and `K`*12+4. They get multiplied, the result is written to `K`\*12+6 and the client terminates.

## Stages
1. The server creates the shared memory object, sets its size, displays its name. After 2 seconds it deletes it and terminates. Client tries to open the shared memory object and displays `Success` or `Failure` accordingly. Then it terminates.
2. The server fills the memory as described in the task. The client reads the correct numbers and writes the multiplication result to `stdout`. At this stage the programs ignore all errors.
3. The server terminates after receiving `SIGINT`. The client writes the multiplication result to the correct address in the shared memory.
4. The server writes the sum of the results to the terminal. The shared memory is deleted when terminating the server. Full error handling.
