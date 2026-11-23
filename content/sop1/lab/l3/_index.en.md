---
title: "L3 - Threads, mutexes and signals"
date: 2022-02-07T19:53:15+01:00
weight: 40
---

# Tutorial 3 - Threads, mutexes and signals

{{< hint info >}}
Introduction notes:

- Quick look at this material will not suffice, you should compile and run all the programs, check how they work, read
  additional materials like man pages. As you read the material please do all the exercises and questions. At the end
  you will find sample task similar to the one you will do during the labs, please do it at home.
- You will find additional information in yellow sections, questions and tasks in blue ones. Under the question you will
  find the answer, to see it you have to click. Please try to answer on you own before checking.
- Full programs' codes are placed as attachments at the bottom of this page. On this page only vital parts of the code
  are displayed
- Codes, information and tasks are organized in logical sequence, in order to fully understand it you should follow this
  sequence. Sometimes former task makes context for the next one and it is harder to comprehend it without the study of
  previous parts.
- Most of exercises require command line to practice, I usually assume that all the files are placed in the current
  working folder and that we do not need to add path parts to file names.
- What you learn and practice in this tutorial will be required for the next ones. If you have a problem with this
  material after the graded lab you can still ask teachers for help.
- This tutorial is based on student's examples, it has some minor flaws in parts not related to the threads (I left them
  on purpose and explain them in comments). Some repeating flaws not mentioned in tasks are:
  - Main functions are too long, it should be easy to split them
  - Bool data type requires extra header and can be easily avoided by using integers (0 and 1)
  - So called  "magic numbers" are left in code - those numerical constants should be replaced with "C" preprocessor macro
{{< /hint >}}

## Thread Management

### Creating Threads
A thread is created using the `pthread_create` command. Let's take a look at the declaration of this function:

```
int pthread_create(pthread_t *restrict thread,
                  const pthread_attr_t *restrict attr,
                  void *(*start_routine)(void*),
                  void *restrict arg);
```

Unlike `fork`, this function returns an integer that is used to determine whether it has succeeded or failed. On success, it writes the newly created thread's handle to `thread`.

The second argument is for attributes. It can be left `NULL` to use the default attributes. (See `man 3p pthread_attr_destroy` and `man -k pthread_attr_set` for additional info)

When creating a thread, you have to supply a function pointer to a function of the following signature:

```
void *function(void *args);
```

This function is the function that will be executed by the newly created thread. You pass the arguments to the function via the last argument `arg`.

In C, a `void*` is a pointer to anything. You can not refer to the underlying value without casting to the actual type, e.g `int*`.

`pthread_t` is implementation defined and should be treated as such. (i.e you can't use it like an integer)

More information:
```
man 3p pthread_create
```

### Thread Joining
Much like child processes, a thread should also be joined after it finishes its work. This can be achieved using the `pthread_join` command.

```
int pthread_join(pthread_t thread, void **value_ptr);
```

`pthread_join` functions similarly to `wait`, blocking until the thread finishes. POSIX doesn't define a non-blocking join function.

Like `pthread_create`, this function returns an integer to indicate success or failure. The thread function's return value is written to `value_ptr` if non-NULL storage was provided.

Unlike with processes, you have to precisely specify which thread you're waiting for by providing a valid thread identifier.

More information:
```
man 3p pthread_join
```

### Excercise

Goal: 
Write a program to approximate PI value with Monte Carlo method, program takes the following parameters:
- k ... number of threads used to approximate,
- n ... number of random tries of each thread.
    
Each thread (except for the main) should conduct its own estimation, 
at the end the main thread gathers the results from co-threads and calculate the average value from all the sub-results. 
        
What you need to know:
- man 7 pthreads
- man 3p pthread_create
- man 3p pthread_join
- man 3p rand (especially  rand_r)
- Monte-Carlo method, in paragraph "Monte Carlo methods" on this <a href="https://en.wikipedia.org/wiki/Pi#Monte_Carlo_methods"> site.</a>

### Solution

**Makefile**:

```makefile
CC=gcc
CFLAGS=-std=gnu99 -Wall -fsanitize=address,undefined
LDFLAGS=-fsanitize=address,undefined
LDLIBS=-lpthread
```

Flag `-lpthread` is mandatory for all compilations in this tutorial. Linked library is called libpthread.so (after -l we
write the name of the library without first "lib" part)

**prog17.c**:
{{< includecode "prog17.c" >}}

### Notes and questions

Functions' declarations at the beginning of the code (not the functions definitions) are quite useful, sometimes
mandatory. If you do not know the difference please
read <a href="http://en.cppreference.com/w/c/language/function_declaration"> this</a>.

In multi threaded processes you can not correctly use `rand()` function, use `rand_r()` instead. The later one requires
individual seed for every thread.

This program uses the simplest schema for threads lifetime. It creates some threads and then immediately waits for them
to finish. More complex scenarios are possible

Please keep in mind that nearly every call to system function (and most calls to library functions) should be followed
with the test on errors and if necessary by the proper reaction on the error.

Instead of `ERR` macro we use a new `HANDLE_ERR`, why?
{{< answer  >}}
Functions that operate on threads (those named like `pthread_*` and some others) do not set `errno`.
Instead, they return an error code.
Using `ERR` would result in error messages like `success: file:line` because `errno` would be set to 0.
It is important to keep this in mind and implement proper error handling for these functions.
{{< /answer >}}

How is input data passed to the new threads?
{{< answer   >}} Exclusively by the pointer to structure targs_t that is passed as thread function arguments. There is no need (nor the excuse) to use global variables! {{< /answer  >}}

Is the thread input data shared between the threads?
{{< answer   >}} Not in this program. In this case there is no need to synchronize the access to this data. Each thread gets a pointer to the private copy of the structure. {{< /answer  >}}

How the random seed for `rand_r()` is prepared for each thread?
{{< answer   >}} It is randomized in the main thread and passed as a part of input data in targs_t. {{< /answer  >}}

In this multi thread program we see srand/rand calls, is this correct? It contradicts what was said a few lines above.
{{< answer   >}} Only one thread is using srand/rand and those functions are called before working threads come to life. The problem with srand/rand and threads originates from one global variable used in the library to hold the current seed. In this code only on thread access this seed thus it is correct. {{< /answer  >}}

Can we share one input data structure for all the threads instead of having a copy for every thread?
{{< answer   >}} Not in this program due to random seed, it must be different for all the threads. {{< /answer  >}}

Can we make the array with the thread input data automatic variable (not allocated)? 
{{< answer   >}} Only if we add some limit on the number of working threads (up to 1000) otherwise this array may use all the stack of the main thread. {{< /answer  >}}

Why do we need to release the memory returned from the working thread?
{{< answer   >}} This memory was allocated in the thread and has to be released somewhere in the same process, The heap is shared by all the threads if you do not release it you will leak the memory. It will not be released automatically on the thread termination. {{< /answer  >}}

Why can't we return the data as the address of local (to the thread) automatic variable? 
{{< answer   >}} The moment thread terminates is the moment of its stack memory release. If you have a pointer to this released stack you should not use it as this memory can be overwritten immediately. What worse, in most cases this memory will stil be the same and faulty program will work in 90% of cases. If you make this kind of mistake it is later very hard to find out why sometimes your code fails. Please be careful and try to avoid this flaw. {{< /answer  >}}

Can we avoid memory allocation in the working thread?
{{< answer   >}} Yes, if we add extra variable to the input structure of the thread. The result can then be stored in this variable.  {{< /answer  >}}

## Detachable threads & Synchronization

### Overview

A detached thread is a thread that you need not join. While it's convenient, it also has downsides, such as not knowing when thread execution ends, as it is not joinable. Because of that, you can't be sure that a thread has actually terminated, which can lead to problems when exiting from the main thread.

Another issue with detached threads is not being able to obtain anything from them without extra synchronization.

There are two ways to obtain a detached thread, those being either spawning a detached thread or manually detaching an already running thread.

### Spawning a detached thread

In order to spawn a detached thread, you must first create a `pthread_attr_t` object that will later be passed to the `pthread_create` function. In order to initialize one to the default state, you must call `pthread_attr_init`. Then, you have to set its detached state to `PTHREAD_CREATE_DETACHED` using the `pthread_attr_setdetachstate` function. Since you have to initialize the attributes structure, you also have to destroy it later using `pthread_attr_destroy`.

More information:
```
man 3p pthread_attr_destroy
man 3p pthread_attr_getdetachstate
man 3p pthread_create
```

### Detaching a running thread

As mentioned above, you may also detach a running thread. You can do it by calling `pthread_detach` with the thread's handle. A thread may detach itself in this way by first obtaining its own handle via a `pthread_self` call, and then calling `pthread_detach`. Attempting to detach a thread that's already detached results in undefined behavior and shouldn't be done.

More information:
```
man 3p pthread_detach
man 3p pthread_self
```

### Mutual exclusion
Since a detached thread on its own is effectively useless, we are introducing shared state between the threads. It means that two or more threads have to work on the same structure. This is problematic, as we currently have no way of ensuring that a thread won't write to a field that is currently being read by another thread. (and vice versa)

This is where mutexes come into play. In short - a mutex is a special object that prevents two or more threads from holding it at the same time. It is recommended to read more about mutexes if you don't know what a mutex is.

You initialize a mutex by calling the `pthread_mutex_init` function. You would later destroy it by calling the `pthread_mutex_destroy` function.
```
int pthread_mutex_init(pthread_mutex_t *restrict mutex,
   const pthread_mutexattr_t *restrict attr);
```
Like `pthread_create`, `pthread_mutex_init` also accepts an attributes object. Similarly, it can be `NULL` to use the default attributes. (See `man 3p pthread_mutexattr_destroy` and `man -k pthread_mutexattr_set` for additional info)



Just creating a mutex isn't enough, you also need to acquire it when accessing shared state. That can be done using `pthread_mutex_lock`. This function blocks until it can acquire the mutex, then tries to acquire it. While it can fail, its failure usually indicates serious memory corruption or very specyfic situation (more on this on OPS2), and thus the return value of this function won't be checked in the further examples and we don't require it during labs.

After you have acquired the mutex, you should not attempt to re-acquire it before releasing. This results in a deadlock in the majority of cases.

After you are done modifying the shared state, you should release the mutex by calling `pthread_mutex_unlock`, so that another thread can lock it. Ideally, you should minimize the time you're holding the mutex for, as it slows down other threads that use it.

Please note that you should NEVER make copies of a mutex (e.g `pthread_mutex_t mtx1_copy = mtx1`).
POSIX forbids that. A copy of a mutex does not have to be a working mutex! Even if it would work, it should be quite obvious, that a copy would be a different mutex.

More information:
```
man 3p pthread_mutex_destroy
man 3p pthread_mutex_lock
```

### Excercise

Goal: 
Write binomial distribution visualization program, use Bean Machine (Galton board) method with 11 bins for bens. The program takes two parameters:
- k ... number of beans trowing threads
- n ... total number of beans to throw

Each thread throws beans separately from others, after each throw, number of beans in bins must me updated. Main thread at each second checks if the simulation has completed (not using thread_join). At the end main thread prints the content of the bins and the mean value of beans. 

What you need to know:

- man 3p pthread_mutex_destroy (full description)
- man 3p pthread_mutex_lock
- man 3p pthread_mutex_unlock
- man 3p pthread_detach
- man 3p pthread_attr_init
- man 3p pthread_attr_destroy
- man 3p pthread_attr_setdetachstate
- man 3p pthread_attr_getdetachstate
- Bean machine on this <a href="https://en.wikipedia.org/wiki/Bean_machine"> site.</a>

### Solution
**prog18.c**:
{{< includecode "prog18.c" >}}

### Notes and questions

Once again, all thread input data is passed as pointer to the structure `thrower_args_t`, treads results modify bins array (
pointer in the same structure), no global variables used.

In this code two mutexes protect two counters and an array of mutexes protects the bins' array (one mutex for every cell
in the array). In total we have BIN_COUNT+2 mutexes.

In this program we use detachable threads. There is no need (nor option) to wait for working threads to finish, thus
the lack of pthread_join. As we do not join the threads, a different method must be deployed to test if the main program can
exit.

In this example mutexes are created in two ways - as automatic and dynamic variables. The first method is simpler in
coding but you need to know exactly how many mutexes you need at coding time. The dynamic creation requires more
coding (initiation and removal) but the amount of mutexes also is dynamic.

Is data passed to threads in `thrower_args_t` structure shared between them?
{{< answer   >}} Some of it, counters and bins are shared and thus protected with mutexes. {{< /answer  >}}

Why do we use a pointer to the `shared_state_t` structure?
{{< answer   >}}  We share the data, without the pointer we would have independent copies of those variables in each thread. {{< /answer  >}}

This program uses a lot of mutexes, can we reduce the number of them?
{{< answer   >}} Yes, in extreme case it can be reduced to only one mutex but at the cost of concurrency. In more reasonable approach you can have 2 mutexes for the counters and one for all the bins, although the concurrency is lower in this case the running time of a program can be a bit shorter as operations on mutexes are quite time consuming for the OS. {{< /answer  >}}

To check if the working threads terminated, the main threads periodically  checks if the numer of thrown beans is equal to the number of beans in total. Is this optimal solution?
{{< answer   >}} No, it is so called "soft busy waiting" but without synchronization tool like a conditional variable (which we will discuss later) it cannot be solved better. {{< /answer  >}}

Do all the threads created in this program really work?
{{< answer   >}} No, especially when there is a lot of threads. It is possible that some of threads "starve". The work code for the thread is very fast, thread creation is rather slow, it is possible that last threads created will have no beans left to throw. To check it please add per thread thrown beans counters and print them on stdout at the thread termination. The problem can be avoided if we add synchronization on threads start - make them start at the same time but this again requires methods that will be introduced later (a barrier or conditional variable). {{< /answer  >}}

## Signals and Threads

Signals are an asynchronous event mechanism in Unix-like operating systems. They allow processes to be notified of system events, exceptions, or requests to control execution.

Each signal has a default action for the process. You can find a list of signals and their default actions in the manual:
```bash
man 7 signal
```

In a multithreaded program, each thread can have its own set of blocked signals.
When a multithreaded program receives a signal, any thread that has not blocked it may receive it.

### Sending signals

The `kill` function sends a signal to a process or group of processes (`man 3p kill`):
```
int kill(pid_t pid, int sig);
```
The `pid` argument specifies the target process or process group:
- `pid > 0` — the signal is sent to the process whose PID is `pid`.
- `pid = 0` — the signal is sent to processes in the sender's process group, including the sender.
- `pid = -1` — the signal is sent to all processes the sender has permission to signal, subject to implementation-defined exclusions; whether the sender itself receives it depends on the implementation.
- `pid < -1` — the signal is sent to processes whose process group ID is the absolute value of `pid`.

The `sig` argument specifies the signal to send. It can be:
- a signal number, preferably one of the macros defined in `<signal.h>`, such as `SIGTERM`, `SIGKILL`, or `SIGUSR1`;
- zero, in which case no signal is sent and the function only checks for possible errors.

The function returns `0` on success. On failure it returns `-1` and sets `errno` accordingly.

POSIX also provides `pthread_kill` to send a signal to a specific thread (`man 3p pthread_kill`):
```
int pthread_kill(pthread_t thread, int sig);
```
Only the selected thread receives the signal. If it is blocked there, the signal remains pending until it is unblocked.

### Handling signals

Historically, signals predate threads. As a result, there are two approaches to handling them: the older one, based on signal handlers, and the newer one, designed for use with threads.

With the older approach, a signal handler is called when a signal arrives, interrupting the program's execution. In single-threaded programs this was effectively the usual way to handle signals, but it has drawbacks. A signal can arrive at any time and interrupt system calls (such as `read` or `write`) or other functions. Some interrupted calls may fail with `errno` set to `EINTR`; others may be automatically restarted, so programs must handle these cases appropriately. This can make programs more complicated.

The older approach uses functions such as `sigaction` (`man 3p sigaction`) and `sigprocmask` (`man 3p sigprocmask`). However, some of these functions are not suitable for multithreaded programs. For example, the manual says that `sigprocmask` is for single-threaded programs, and its behavior in a multithreaded program is unspecified. Because of this and the other problems mentioned above (including the need for global variables), we will not use signal handlers in the lab.

Instead, we will discuss a newer approach that delegates signal handling to a dedicated thread. The general pattern is:
1. Block the signals in all threads.
2. Create a thread that waits for signals in a loop (it is the only thread that can receive them).
3. When it receives a signal, that thread notifies the other threads, for example through a shared variable or other mechanisms that we will learn about later.

### Setting a signal mask

In a multithreaded program, use `pthread_sigmask` to set the signal mask. It sets the mask for the calling thread (`man 3p pthread_sigmask`).

Threads inherit the signal mask from the thread that created them.

To block a signal throughout the program, block it in every thread. Otherwise, an unblocked thread may receive it; if no process-wide handler is installed, the default action may terminate the program.

The `pthread_sigmask` function specifies how a mask affects the calling thread's current signal mask. Its declaration is:
```
int pthread_sigmask(int how, const sigset_t *restrict set,
           sigset_t *restrict oset);
```
The arguments are:
- `how` specifies how to change the current mask:
  - `SIG_BLOCK` — the resulting mask is the union of the current mask and `set` (add these signals to the mask).
  - `SIG_SETMASK` — the resulting mask is `set`.
  - `SIG_UNBLOCK` — the resulting mask is the current mask with the signals in `set` removed.
- `set` points to the mask used to update the current mask.
- `oset` points to an object where the previous signal mask is stored.

For example:
```C
sigset_t mask, oldmask;
sigemptyset(&mask);
sigaddset(&mask, SIGUSR1);
pthread_sigmask(SIG_BLOCK, &mask, &oldmask);
```
This adds `SIGUSR1` to the blocked signals of the calling thread.

### Waiting for a signal

Use `sigwait` to wait for a signal (`man 3p sigwait`):
```
int sigwait(const sigset_t *restrict set, int *restrict sig);
```
The first argument is the set of signals to wait for. These signals should be blocked in the calling thread.
The output argument `sig` receives the number of the signal accepted by `sigwait`.

### Excercise
Goal: 
The program takes sole 'k' parameter and prints the list of numbers form 1 to k at each second. It must handle two signals in dedicated thread, the following action must be taken upon the signal arrival:
- SIGINT  (C-c) ... removes random number from the list (do nothing if empty),
- SIGQUIT  (C-\\) ... set program 'STOP'  flag (both threads end).

What you need to know:

- man 3p pthread_sigmask
- man 3p sigwait

**prog19.c**
{{< includecode "prog19.c" >}}

### Notes and questions

Thread input structure argsSignalHandler_t holds the shared threads data (an array and STOP flag) with protective
mutexes and not shared (signal mask and tid of thread designated to handle the signals).

How many threads run in this program?
{{< answer   >}} Two, main thread created by system (every process has one starting thread) and the thread created by the code.  {{< /answer  >}}

Is waiting for the thread as inefficient in this program as in the previous one?
{{< answer   >}} No, checking every second is part of the task. If it were not, we could avoid busy waiting by synchronizing with `pthread_join`. {{< /answer   >}}

Why do we not check errors from calls that acquire and release mutexes?
{{< answer   >}} The basic mutex type used in this program does not check or report errors. {{< /answer   >}}


## Thread cancelation

### Canceling a thread

A thread can be canceled using the `pthread_cancel` function.
This is useful when the program has to exit before it finishes its work, for example due to a signal.

More information:
```
man 3p pthread_cancel
```

### Setting cancelability

A thread can choose how it will respond to cancelation requests using the following functions:

```
int pthread_setcancelstate(int state, int *oldstate);
int pthread_setcanceltype(int type, int *oldtype);
```

Cancelation requests can be ignored by setting the cancel state to `PTHREAD_CANCEL_DISABLE` and then later re-enabled by setting it to `PTHREAD_CANCEL_ENABLE`.
Cancelation is enabled by default.

The cancel type determines when the thread exits upon receiving a cancelation request.
It is set to `PTHREAD_CANCEL_DEFERRED` by default, in which case the thread exits at a cancelation point.
If it is set to `PTHREAD_CANCEL_ASYNCHRONOUS`, the thread will exit as soon as possible.

Generally, it is safer to keep the cancel type as `PTHREAD_CANCEL_DEFERRED`,
as you may have an unpreventable race condition in the case of asynchronous cancelation
(e.g a thread gets canceled after you lock a mutex but before you add a cleanup function)

More information:
```
man 3p pthread_setcancelstate
man 7 pthreads (specifically the "Cancelation Points" section)
```

### Cleanup functions

In a lot of cases, exiting a thread mid-execution leaks resources.
This is problematic, especially if a thread is holding a mutex - as it won't be released.

To solve this issue, POSIX allows creating cleanup handlers that run when the thread has to exit abruptly. (i.e due to being cancelled or calling `pthread_exit`)

Those take form of a function with the following signature:
```
void function(void* arg);
```

You can add this function as a cleanup handle by issuing the `pthread_cleanup_push` call. Let's take a look at the function declaration.
```
void pthread_cleanup_push(void (*routine)(void*), void *arg);
```
As you can see, the function accepts the aforementioned function along with the argument that will be passed to it.

When a thread exits abruptly, all of the cleanup functions are executed in the reverse order of their addition.

You also can remove cleanup handlers by issuing `pthread_cleanup_pop`.
```
void pthread_cleanup_pop(int execute);
```
This function removes the handlers in the same order as they would be executed. 
The `execute` parameter allows for optional execution of the handler.

For every instance of pthread_cleanup_push in a scope there must be an instance of pthread_cleanup_pop.

More information:
```
man 3p pthread_cleanup_pop
```

### Joining canceled threads

Canceled threads remain joinable, since a cancel request isn't guaranteed to be processed.

If a thread gets canceled, the value pointer passed to `pthread_join` will be set to `PTHREAD_CANCELED`

More information:
```
man 3p pthread_join
man 3p pthread_exit (specifically the last paragraph of the RATIONALE section)
```

### Excercise

Goal: 
Program simulates the faith of MiNI students, it takes the following parameter:
- n <= 100 ... count of new students

The program stores the counters of students studding on year 1,2,3 and the final BSc year.

Main Thread: Initiate students, then for 4 seconds at random intervals (100-300ms) strike off one of the students (cancel thread). After the 4s period waits for the students threads and prints the counters. 

Students threads: Student (thread) adds itself to the counter of the first year, after a second it removes itself from it and adds to the second year counter and so on until it reaches the BSc counter.  The thread must be always prepared for cancellation.

What you need to know:

- man 3p pthread_cancel
- man 3 pthread_cleanup_push
- man 3 pthread_cleanup_pop
- man 7 time
- man 3p clock_getres

**prog20.c**:
{{< includecode "prog20.c" >}}

### Notes and questions
Threads receive the pointer to the structure with current year and pointer to years counters, structure argsModify_t
does not have the same flow as one in task 2 of this tutorial i.e. program is not making too many unnecessary references
to the same data.

Structure `studentsList_t` is only used im main thread, it is not visible for students' threads.

Cleaver structure yearCounters_t initialization will not work in archaic C standards (c89/c90). It is worth knowing but
please use all the improvements of newer standards in your code.

Cleanup handlers in working thread are deployed to safely expel the student while its thread is asleep. Without the
handlers the canceled student will occupy the last counter till the end of the program!

Please keep in mind that pthread_cleanup_push must be paired with pthread_cleanup_pop in the same lexical scope (the
same braces {}).


How many mutexes this program uses and what they protect?
{{< answer   >}} Four exactly, one for each year/counter. {{< /answer  >}}

Must current year of a student be a part of argsModify_t structure?
{{< answer   >}} No it could have been automatic in thread variable, then structure argsModify_t would be no longer needed and you would pass pointer to  yearCounters instead. {{< /answer  >}}

What does it mean that the thread cancellation state is set to PTHREAD_CANCEL_DEFERRED ?
{{< answer   >}} Cancellation will only happen during certain function calls (so called cancellation points), it will not disturb the rest of the code in the thread. In other words the thread can finish a part of its work before it is canceled. {{< /answer  >}}

What functions used in the thread code are cancellation points?
{{< answer   >}}  Only nanosleep (called from msleep) is a cancellation point in this code. {{< /answer  >}}

How do we learn what functions are cancellation points?
{{< answer   >}} $man 7 pthreads {{< /answer  >}}

What one in this call " pthread_cleanup_pop(1);" means ?
{{< answer   >}} It means that the handler is not only removed from the handlers stack but also executed. {{< /answer  >}}

When the year counter is decreased?
{{< answer   >}} In two cases, during cancellation (rare case), during the removal of cleanup handler from the stack of handlers. {{< /answer  >}}

Algorithm selecting a thread for cancellation has an apparent flow, can you name it and tell what threat it creates?
{{< answer   >}} This random selection can last very long if only a few "live" threads are left on a large list of threads. Try to run the program with 10 as the parameter to check it. {{< /answer  >}}

Improve random selection as an exercise.

Have a look at the method used to measure the 4 seconds life time of the program (clock_gettime, nanosleep). Please change the solution  to use alarm function and the SIGALRM handler as an exercise.

Do the example tasks. During the laboratory you will have more time and a starting code. If you do following tasks in the allotted time, it means that you are well-prepared.

- [Task 1]({{< ref "/sop1/lab/l3/example1" >}}) ~60 minutes
- [Task 2]({{< ref "/sop1/lab/l3/example2" >}}) ~120 minutes
- [Task 3]({{< ref "/sop1/lab/l3/example3" >}}) ~100 minutes

## Source codes presented in this tutorial

{{% codeattachments %}}
