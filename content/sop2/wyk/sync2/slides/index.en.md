---
title: "Slides"
type: presentation
layout: single
---

# Applications of Synchronization

Operating Systems 2 Lecture

<small>Warsaw University of Technology<br/>Faculty of Mathematics and Information Science</small>

---

### POSIX Semaphores

POSIX defines the following API:
* `sem_t`: type holding semaphore object.
* `sem_open(path)`, `sem_init(&s, val)`: methods of obtaining `sem_t`.
* `sem_wait(&s)`: Decrement. If 0, put the process to sleep (wait).
* `sem_post(&s)`: Increment. Wake up one waiting process.

---

### Where's my semaphore?

`sem_t` value might live in:
* private process memory (stack, heap, global)
* shared memory segment
* system-wide semaphore namespace

---

### Unnamed Semaphores

Used to synchronize threads within the **same process**. The `sem_t` object lives in standard private memory (e.g., a global variable, heap, or `main`'s stack).

* **Initialization:** `sem_init(&sem, /*pshared*/0, initial_value)`
* **Persistence:** Volatile. Tied strictly to the process memory. When the process terminates (or memory is freed), the semaphore ceases to exist.

---

```c
sem_t my_sem; 

void* worker(void* arg) {
    sem_wait(&my_sem);
    // ... thread-safe critical section ...
    sem_post(&my_sem);
    return NULL;
}

sem_init(&my_sem, 0, 1); 
// ... create threads, wait for them ...
sem_destroy(&my_sem);
```

---

### Unnamed Semaphores in SHM

To synchronize processes, the `sem_t` object **must** be placed in a shared memory region (e.g., created via `shm_open`).

* **Initialization**: `sem_init(&sem, /*pshared*/1, initial_value)`
* **Persistence**: Depends entirely on the underlying memory object.

---

```c
struct shm {
    sem_t sem;
    char buffer[100];
};

int fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0666);
ftruncate(fd, sizeof(struct shm));
sem_t* shared_sem = mmap(NULL, sizeof(struct shm), 
                         PROT_READ | PROT_WRITE, 
                         MAP_SHARED, fd, 0);
sem_init(shared_sem, 1, 1);
sem_wait(shared_sem);
sem_post(shared_sem);
// ...
```

---

### Named Semaphores

Useful when processes just want to synchronize without explicit memory sharing.

* **Initialization**: `sem_open("/name", O_CREAT, mode, initial_value)`
* **Persistence:** Kernel Persistence. Lives until explicitly destroyed via `sem_unlink()` or a system reboot.

```c
sem_t* my_sem = sem_open("/my_sem", O_CREAT, 0666, 1);
sem_wait(my_sem);
sem_post(my_sem);
sem_unlink("/my_sem");
```

In Linux, named sempahores are visible through `/dev/shm` virtual FS.

---

### What `sem_t` really is?

`sem_t` is an _opaque type_ - should be treated as a black box. 
Let's have a look into the typical glibc implementation:

```c
struct internal_sem {
    uint64_t data; // [ nwaiters (4b), value (4b) ]
    int private;   // `pshared` flag
    int pad;
};
```
See for yourself on [GitHub](https://github.com/lattera/glibc/blob/895ef79e04a953cac1493863bcae29ad85657ee1/sysdeps/nptl/internaltypes.h#L140).

Semaphore implementation uses `data` field as futex word, taking advantage of userspace fast-paths.
It combines two elements: the actual semaphore value and number of waiters.

---

### Why `pshared`?

When `pshared = 0` implementation passes `FUTEX_PRIVATE_FLAG` to `futex()` syscalls. This instructs the kernel
to use optimized approach - it does not need to be concerned with situation where same futex word is mapped
at different virtual addresses.

![pshared.svg](/ops2/wyk/sync2/pshared.svg)

---

### Named Semaphores Illusion

There is no magical, separate synchronization entity in the kernel. It is simply a convenience wrapper provided by the C library.

1. **File Creation:** It creates a small POSIX shared memory object, visible as `/dev/shm/sem.<my_sem>`.
2. **Memory Mapping:** It uses `mmap()` to map this tiny file into your process's virtual address space.
3. **Initialization:** It places a standard `sem_t` structure inside that mapped memory and initializes it with `pshared = 1`.

---

### Bounded Buffer Problem

A classic synchronization paradigm. Producer(s) want to send work items to consumer(s) through a shared buffer of a fixed size ($N$ slots).

![bb.svg](/ops2/wyk/sync2/bb.svg)

Buffer may be empty (blocking consumers), or full (blocking producers).

This is exactly what's solved by I/O streams, Pipes/FIFOs (`|`), audio streaming buffers, task queues in thread pools.

---

### Buffer implementation

The buffer is easily implemented using _circular buffer_ approach. This limits data moves upon consumption.

![circularbuffer.svg](/ops2/wyk/sync2/circularbuffer.svg)

---

### The Semaphore Solution

Semaphores are perfect here because they inherently **count** resources. We will use three of them:

1. `mutex`: Initialized to 1. Provides exclusive access to the buffer.
2. `empty`: Initialized to `N` (capacity). Counts available free slots.
3. `full`:  Initialized to 0. Counts available unread items.

*Note:* At any given time, the value of `empty` + `full` will always be `< N`

---

Producer
```c
sem_wait(&empty);    // 1. Wait for a free slot
sem_wait(&mutex);    // 2. Lock the buffer
buffer[in] = item;
in = (in + 1) % N;
sem_post(&mutex);    // 3. Unlock the buffer
sem_post(&full);     // 4. Make new item available
```
Consumer
```
sem_wait(&full);     // 1. Wait for an available item
sem_wait(&mutex);    // 2. Lock the buffer
item = buffer[out];
out = (out + 1) % N;
sem_post(&mutex);    // 3. Unlock the buffer
sem_post(&empty);    // 4. Make new free slot available
```

---

### Shared Mutexes

`pthread` library _synchronization primitives_ can also be used by multiple processes
if put inside shared memory mapping.

```c
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
// Tell the OS that the mutex will be shared
pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)

pthread_mutex_init(&shm_ptr->mutex, &attr);
pthread_mutexattr_destroy(&attr);
```

Now we can use `pthread_mutex_lock()` in Process A and `pthread_mutex_unlock()` in Process B!

---

### Mutex placement

As in case of semaphores, mutex values protecting shared data are usually placed in the same shared memory segment as the protected data itself.

```c
struct shm_layout {
    pthread_mutex_t mtx; // protects everything!
    int active_processes;
    int pids[MAX_PROCESSES];
    // ...
};

struct shm_layout* shm = mmap(...);
```

Mutex has to be initialized once, usually creator of the shm segment is responsible.

Distinct parts of single shm segment may be protected by distinct mutex instances. There are various application-specific
locking patterns possible: cell-level, row-level, data structure-level, etc.

---

### Pthread mutex types

The default mutex (so-called fast) is dumb. 
If you lock it twice in a row within the same thread, you will deadlock with yourself.

Other types (configured with `pthread_mutexattr_settype()`):
* **Error checking**: Detects an attempt to double-lock and returns an `EDEADLK` error instead of hanging. Great for debugging!
* **Recursive**: Allows one process to acquire the lock multiple times (maintains an internal counter). Requires the exact same number of unlock calls. Essential in recursive functions modifying shared state.

---

### Uses of Recursive Mutex

Why would a thread want to lock a mutex it already holds?

1. **Recursive Data Structures**

Recursive shared tree or graph node traversal subroutine locks a mutex and calls itself, a standard fast mutex will instantly deadlock on the first recursion.

2. **Composed Non-orthogonal Operations**:

`foo()` locks the mutex, updates a variable, and unlocks. `goo()` of the same library locks the mutex, does some complex work, and then... calls `foo()` to update that same variable.

---

### IPC Mutex Disaster

Process A calls `pthread_mutex_lock()`. It enters the critical section.
Suddenly... a `SEGFAULT` occurs in A (or someone runs kill -9). Mutex owner dies.

What happens to the Mutex in SHM?
**It stays locked. Forever.**

Other processes B, C, and D wait infinitely. The entire IPC halts.

---

### Robust Mutex

The solution: `PTHREAD_MUTEX_ROBUST` attribute.

```c[2-3]
pthread_mutexattr_init(&attr);
pthread_mutexattr_setrobust(&attr,
    PTHREAD_MUTEX_ROBUST);
pthread_mutex_init(&mtx, &attr);
```

The OS kernel carefully tracks which process holds this mutex. 
If the process dies without releasing the lock, the system "wakes up" the next waiting process.

`pthread_mutex_lock()` succeeds but returns a special error code: `EOWNERDEAD` saying:

> "Hey, the previous owner of the lock died. They probably left a mess in the data structures. Handle it."

---

### Cleaning up after a dead owner

```c
int ret = pthread_mutex_lock(&shm->robust_mutex);

if (ret == EOWNERDEAD) {
    // ... repair corrupted data structures here ...
    
    // "I cleaned up, all good!"
    pthread_mutex_consistent(&shm->robust_mutex);
}

// ... regular critical section ...
pthread_mutex_unlock(&shm->robust_mutex);
```

This is the foundation of building fault-tolerant IPC systems!

Omitting call `pthread_mutex_consistent()` breaks the lock permanently. All subsequent lock calls
will return `ENOTRECOVERABLE`. It's vital to handle return values in IPC code!

---

### Conditional variables recap

Unlike semaphores (which count internally), CVs are completely stateless.

Recall usage pattern of `pthread_cond_wait(&cv, &mtx)`.
It performs three steps atomically from the caller's perspective:
- unlocks the `mtx` 
- puts the caller to sleep on the cv queue
- (now others can enter the critical section!)
- (... time passes ... someone calls signal or broadcast)
- re-acquires the `mtx`
- wakes up the caller

---

### Bounded Buffer (CV Edition)

```c
pthread_mutex_lock(&buffer->mtx);
while (buffer->size >= BUFFER_SIZE)
    pthread_cond_wait(&buffer->empty, &buffer->mtx);
// produce item, size increases
pthread_cond_signal(&buffer->nonempty);
pthread_mutex_unlock(&buffer->mtx);
```

Producer waits util buffer has empty slot using `empty` CV, the signals possibly waiting consumers:

```c
pthread_mutex_lock(&buffer->mtx);
while (buffer->size == 0)
    pthread_cond_wait(&buffer->nonempty, &buffer->mtx);
// consume item, size decreases
pthread_cond_signal(&buffer->empty);
pthread_mutex_unlock(&buffer->mtx);
```

---

### Why while and not if?

Notice usage of `while (buffer->size ...)` as awaited condition. This is mandatory for two reasons:

**Signal Stealing**: 

Process A is sleeping. Process B puts an item and signals. P.rocess A wakes up and tries to re-acquire the mutex. 
But Process C (a fast newcomer) grabs the mutex first, consumes the item, and leaves. When Process A finally gets the lock, the buffer is empty again!

**Spurious Wakeups**: 

`pthread_cond_signal()` wakes up **at least** one waiter. `pthread_cond_wait()` may return due to signal interrupt.

---

### The "Monitor" Pattern

A _Monitor_ is a synchronization construct consisting of:

- encapsulated private data (the buffer array, indices, count).
- public access methods (operations)
- a mutex guaranteeing exclusive access to the private data.
- one or more Condition Variables to handle waiting for state changes.

Some languages like Java have monitors built-in (`synchronized` keyword, `wait()`, `notify()`).
In C and POSIX IPC, we build them manually using structs, `pthread_mutex_t`, and `pthread_cond_t`.


---

### Java monitor example

```java
public class bank {
    double accounts[100];
    public synchronized double current_balance(int i) {
        return accounts[i];
    }
    public synchronized void transfer(double amount, 
      int source, int target) {
        while (accounts[source] < amount) { wait(); }
        accounts[source] -= amount;
        accounts[target] += amount;
        notifyAll();
    }
}
```

---

### Readers-Writers Problem

Consider a mutable data structure placed in Shared Memory:
- **Readers**: Some processes access it in read only manner. They don't interfere with each other.
- **Writers**: Some must have exclusive access as they mutate it.

There are two variations of this problem:

1. **Favoring readers**: As long as at least one reader is reading, we let new ones in.

Danger: Writer Starvation.

2. **Favoring writers**: As soon as a writer shows up, new readers are blocked.

Danger: Reader Starvation.

---

### Semaphore solution

Writer

```c
wait(write_sem);
/* write */
signal(write_sem);
```

Reader

```c
wait(read_sem);
read_count++;
if (read_count == 1) wait(write_sem);
signal(read_sem);
/* read */
wait(read_sem);
read_count--;
if (read_count == 0) signal(write_sem);
signal(read_sem);
```

---

### CV solution (Writer)

```c
pthread_mutex_lock(&mtx);
while (readers_count > 0 || writers_count > 0)
    pthread_cond_wait(&cv, &mtx);
writers_count++;
pthread_mutex_unlock(&mtx);
/* read */
pthread_mutex_lock(&mtx);
writers_count--;
pthread_cond_signal(&cv);
pthread_mutex_unlock(&cv);
```

---

### CV solution (Reader)

```c
pthread_mutex_lock(&mtx);
while (writers_count > 0)
    pthread_cond_wait(&cv, &mtx);
readers_count++;
pthread_mutex_unlock(&mtx);
/* read */
pthread_mutex_lock(&mtx);
readers_count--;
pthread_cond_signal(&cv);
pthread_mutex_unlock(&cv);
```

Favorizing writers may be implemented by extending shared state with `writers_waiting` counter.

---

### RW locks

POSIX introduces dedicated synchronization primitive solving RW problem: `pthread_rwlock_t`.

```c
pthread_rwlock_rdlock(&rwlock)
/* read */
pthread_mutex_unlock(&rwlock);
```

```c
pthread_rwlock_wrlock(&rwlock)
/* write */
pthread_mutex_unlock(&rwlock);
```

_Pro-tip_: By default, Linux glibc's `pthread_rwlock_t` favors readers. 
If you want to prevent writer starvation, you can configure it using the non-standard `pthread_rwlockattr_setkind_np()` attribute!