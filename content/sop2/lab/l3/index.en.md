---
title: "L3 - Shared memory and mmap"
weight: 30
---

# Shared memory and mmap

{{< hint info >}}

This is the first tutorial in the "new formula". It contains more explanations for the used functions and their parameters. We hope that this format will serve you better for this class. It is still only a surface-level tutorial and it is **vital that you read the man pages** to familiarize yourself and understand all of the details.

{{< /hint >}}


## mmap

Last time we went over a few methods of synchronization and data sharing between processes. They always seem more complicated than in the case of threads - where we can simply have shared variables. However, when we use the `fork()` function, the child processes receive their own copy of the parent's data. Thus, when we modify an array created before the call fo `fork()` - changes will occur only within the child process.

It is possible, however, to declare a "mapping" between the program memory and a "memory object". All changes made in the mapped area will carry over into the mapped "memory object", managed by the operating system. To create such an object, we use `mmap` (`man 3p mmap`, `man 2 mmap`).

This function has 2 main uses:

- Mapping a file onto an area of memory - it allows us to modify the contents of a file as if it was a simple array.
- Creating an area of memory to be shared with child processes.
- Mapping an area of shared memory (more will be explained in the second part of this tutorial).

Let's look at the signature of this function:

```
void *mmap(void *addr, size_t len, int prot, int flags,
           int fildes, off_t off);
```

- `addr` - this parameter allows us to inform the operating system where in memory we want the map to be. In our use cases we pass `NULL` for the system to decide for us.
- `len` - means the size of the memory we want to map.
- `prot` - operations (read/write/execute - see man page) we want to do on the memory - similarily to files.
- `flags` - this parameter determines the kind of mapping created by `mmap`. The most important flags are described below, read `man 2 mmap` to familiarize yourself with the full list and its details.
  - `MAP_ANONYMOUS` - we create a new anonymous area of memory. When using this flag, the parameter `filedes` should be set to `-1`. When this flag is not used, the function maps a file by default.
  - `MAP_SHARED` - the resulting mapping is shared with different processes. In our case we almost always want to use this flag.
  - `MAP_PRIVATE` - the opposite of the former flag - the resulting mapping is not shared, the memory will be copied in the child processes. Not very useful in our case, but still important. Have you ever wondered how `malloc` works?
- `filedes` - descriptor of the file we want to map.
- `off` - offset in the file we want to map. With `MAP_ANONYMOUS` it should always be set to 0.

That's kind of it - the pointer to memory received from calling `mmap` with the flag `MAP_SHARED` will be usable in any child process and changes made will be visible in all other processes. However, one needs to consider either synchronisation of memory accesses or restricting each process to work only in its own dedicated area of the shared memory.

There are two other functions related to `mmap`:

- `msync` - useful when mapping files, it forces the operating system to modify the underlying file. For the sake of efficiency whenever we modify memory that is mapped to a file, the system won't immediately apply those changes. It is done this way so that there aren't many small changes significantly slowing down the system. If we want to make sure that our changes are written to the mapped file, we use `msync`. You can think about it as analogous to `fflush` for streams. See `man 3p msync`.
-`munmap` - mapped memory has to be "unmapped" - that's what this function is for - see `man 3p munmap`. It's worth mentioning that with mapped files `munmap` doesn't guarantee synchronisation - calling `munmap` is needed to guarantee that our changes are applied.

### Excercise

Write a program approximating PI using the Monte Carlo method. It takes one argument -  `0 < N < 30` - number of calculating processes. Every one performs `100 000` Monte Carlo iterations.

The main process maps two areas of memory. The first one is used for sharing the results of child processes' calculations. It is N*4 bytes wide. Every child process saves the result of its calculations to one 4-byte cell as a float. The second area of memory maps the file `log.txt`. The main process sets its size to `N*8`. The child processes save their approximations there in text form. Each one in a single line of width 7 (+eigth symbol `\n`).

### Solution

New man pages:
```
man 3p mmap
man 2 mmap
man 3p munmap
man 3p msync
man 0p sys_mman.h
man 3p ftruncate
```

solution `l3-1.c`:
{{< includecode "l3-1.c" >}}

### Notes and questions 

- Why isn't synchronisation used?
{{< answer >}} Every child process writes to its own dedicated fragment of shared memory and then the parent joins the results. That's why no conflict is possible and no synchronsation is required. {{< /answer >}}

- Why is a `ftruncate` call required?
{{< answer >}} Without it the `log.txt` file would be size 0, because it's opened with the `O_TRUNC` flag, which causes thefunction to remove the file's contents. Otherwise, if the flag wouldn't have been added, the size would have been dependent on the earlier contents, which would have been unpredictable. {{< /answer >}}

## Shared memory and robust mutex

### shm_*

In the example above we created an area of memory using `mmap`, which was shared by a group of processes created using `fork()`. What if we wanted the processes to be independent? We can create a named shared memory object, which can be mapped by any process using its unique name. It is very similar and analogous in their differences to the pair: pipe - FIFO, which was introduced in L1.

To create a named shared memory object use `shm_open` (see `man 3p shm_open`). Its usage is very simple, and the parameters are analogous to regular `open` or `mq_open`, so reading the man page should explain it.

`shm_open` returns a descriptor, which behaves very similarily to a file descriptor. Initially the created memory is of size zero, so it is required to increase it with e.g. `ftruncate`. Then simply use the function `mmap`.

When all processes are done using the shared memory it should be deleted using the `shm_unlink` function. It is also easy, so just check `man 3p shm_unlink`.

### Sharing synchronisation objects and the robust mutex

We can synchronise access to shared memory with the tools we already know - pipes, fifo and message queues. However, it's also possible to place semaphores and mutexes in the shared memory, so that all proceses have access to them.

For semaphores it's easy: see `man 3p sem_init`. If the second parameter `pshared` is set to a value other than zero, the semaphore can be used across processes. It's enough, then, to choose an area of the shared memory of size `sizeof(sem_t)`, initialise a semaphore that way and all processes will be able to use it.

Sharing mutexes is a little harder. Setting the special properties of a mutex is also done during initialisation (`pthread_mutex_init`, but see `man 3p pthread_mutex_destroy`) - we can pass special attributes as the second parameter, `pthread_mutexattr_init`. Then, for the mutex to be shared between processes, a mutex attribute object should be created (`pthread_mutexattr_init`), `pthread_mutexattr_setpshared` should be called (see `man 3p pthread_mutexattr_getpshared`), and finally those attributes should be passed to `pthread_mutex_init`.

A mutex created this way will be shared between processes correctly, but it is not the end of our troubles. The basic limitation of the mutex having to be unlocked in the same thread that locked it is still in power. It creates a problem whenever a process terminates due to an error after locking the mutex and before unlocking it. The robust mutex solves this problem.

Creating a robust mutex requires passing the appropriate attribute set with `pthread_mutexattr_setrobust` (see `man 3p pthread_mutexattr_getrobust`) - similarly to sharing. When we attempt to lock (`pthread_mutexattr_setrobust`) a mutex in an invalid state (locked by a process that terminated already) we will get the `EOWNERDEAD` error (see `man 3p pthread_mutex_lock`), but the mutex will get **locked** anyway. To repair the mutex's state `pthread_mutex_consistent` (`man 3p pthread_mutex_consistent`) needs to be called.

Condition variables and barriers are initialised similarily. They can also be shared between processes when appropriate attributes are set. See `man 3p pthread_condattr_getpshared` and `man 3p pthread_barrierattr_getpshared`. Those objects thankfully don't have additional problems that need to be addressed like in the mutex case.

See how shared memory and mutexes were used in the excercise below.

### Excercise

Write two programs - client and server. The server takes one parameter - `3 < N <= 20`. It writes `My PID is: <pid>` to the terminal and creates a segment of shared memory with the name `<pid>-board` that is 1024 bytes wide. It fills that memory with a mutex, `N` and the board - an array of NxN bytes. The board is filled with random numbers from the `[1,9]` range. Every 3 seconds it displays the board's state to the terminal. After receiving `SIGINT` it displays the board one last time and terminates.

The client program takes one parameter - `PID` of the server. It opens the memory created by the server. Then it follows the instructions:

1. Lock the mutex.
2. Pick a random number from the `[1,10]` range. If `1` is picked, it prints `Oops...` and terminates.
3. Pick 2 numbers `x` and `y` from `0` to `N-1` and writes `trying to search field (x,y)` to the terminal.
4. Check what number is on the board under `(x,y)`.
5. If it's not zero, add the number to your score, write `found <P> points` to the terminal, zero that board space, unlock the mutex, wait a second and go back to step 1.
6. If it's zero, the program unlocks the mutex, writes `GAME OVER: score <X>` (where `X` is the score) to the terminal and terminates.


### Solution

New man pages:
```
man 3p shm_open
man 3p shm_unlink
man 3p pthread_mutexattr_destroy
man 3p pthread_mutexattr_setpshared
man 3p pthread_mutexattr_setrobust
man 3p pthread_mutex_consistent
```

solution `l3-1_server.c`:
{{< includecode "l3-2_server.c" >}}

solution `l3-1_client.c`:
{{< includecode "l3-2_client.c" >}}

### Notes and questions

- Pay attention to the way signals are handled. By using a dedicated thread, the problematic code for signal handling is contained in one function, there are no global variables and we don't have to use `TEMP_FAILURE_RETRY` everywhere. It matters a lot, especially in complex projects.

- If not for the random termination of the client processes, would the robust mutex still be needed?
{{< answer >}} Yes, the process could still terminate here e.g. by receiving a `SIGINT`. Generally when sharing a mutex with different processes, it's best to always make it robust. {{< /answer >}}

- Does the robust mutex make it so that a deadlock due to an error is impossible? 
{{< answer >}} Unfortunately no. The process can still fail to release a mutex or have a different error. Note the special case where a process `unmap`s the shared memory before releasing the mutex. The "robustness" doesn't work here! {{< /answer >}}

- Why is a call to `ftruncate` required?
{{< answer >}} A shared memory object is zero-sized when created and has to be resized. {{< /answer >}}

## Named semaphore

Named semaphores were already mentioned in SOP1, but it's probably worth to revise. A named semaphore is related to an unnamed one the same way that a fifo is related to a pipe. We create it with the `sem_open` function (see `man 3p sem_open`) passing the `O_CREAT` flag, access rights and an initial value. E.g.:

```
sem_t *semaphore = sem_open("SOP-semaphore", O_CREAT, 0666, 5);
```

Will create a semaphore with the name `SOP-semaphore` and initialise it with a value of 5. Calling the function with the same parameters (or without `O_CREATE`: `sem_open("SOP-semaphore", 0);`) in another process will return the same, existing semaphore. As you can see it's analogous to fifo and message queues.

Thanks to their simplicity, named semaphores are great for the synchronisation of processes sharing memory. When a few independent processes want to use a single object only one is going to initialise it. It's important for the other processes to wait until the initialisation is done - which can be accomplished simply with a semaphore.

## Source code from the tutorial
{{% codeattachments %}}

## Additional materials

- Try to solve the example [excercise from L1]({{% ref "/sop2/lab/l1-task-normal" %}}) using shared memory. Instead of sending messages that a new round has started synchronisation can be accomplished with a semaphore (parent calls `sem_post` the appropriate amount of times at the start of a round). Test the semaphore shared within shared memory and the named semaphore opened independently by child processes. Instead of sending cards by pipes players can place them in the appropriate place within shared memory. For notifying the server that players placed their cards you can use a shared memory barrier or a second semaphore.

- <http://cs341.cs.illinois.edu/coursebook/Ipc#mmap>
