---
title: "L2 - Processes, Signals and Descriptors"
date: 2022-02-07T19:46:03+01:00
weight: 30
---

# Tutorial 2 - Processes, Signals and Descriptors

{{< hint info >}}
Introduction notes:

- Quick look at this material will not suffice, you should compile and run all the programs, check how they work, read additional materials like man pages. As you read the material please do all the exercises and questions. At the end you will find sample task similar to the one you will do during the labs, please do it at home.
- You will find additional information in yellow sections, questions and tasks in blue ones. Under the question you will find the answer, to see it you have to click. Please try to answer on you own before checking. 
- Full programs' codes are placed as attachments at the bottom of this page. On this page only vital parts of the code are displayed
- Codes, information and tasks are organized in logical sequence, in order to fully understand it you should follow this sequence. Sometimes former task makes context for the next one and it is harder to comprehend it without the study of previous parts.  
- Most of exercises require command line to practice, I usually assume that all the files are placed in the current working folder and that we do not need to add path parts to file names. 
- Quite often you will find $ sign placed before commands you should run in the shell, obviously you do not need to rewrite this sight to command line, I put it there to remind you that it is a command to execute.problem with this material after the graded lab you can still ask teachers for help. 
- This time some of the solutions are
- What you learn and practice in this tutorial will be required for the next ones. If you have a  divided into two stages
{{< /hint >}}

## Process Management

### Creating Processes
A child process is created using the `fork` command. Let’s take a look at the definition of this function:

```
pid_t fork(void)
```

As you can see, it returns an object of type `pid_t`, which is a signed integer type. In the parent process, the function returns the identifier of the newly created process, while in the child process it returns `0`. This makes it easy to distinguish between the logic executed by the child and the logic executed by the parent.

Of course, creating a new process may fail (for example, when the system runs out of necessary resources). In such a case, the `fork` function returns `-1` and sets the appropriate value of the `errno` variable.

Processes created by a given process are called its __children__, while from the perspective of a child process, the process that created it is called the __parent__.

### Process Identification
Each process has a unique identifier of type `pid_t`. To obtain information about the process identifier, we use the `getpid()` function, and to find out the identifier of the parent process, we use the `getppid()` function. Their definitions are as follows:

```
pid_t getpid(void)
pid_t getppid(void)
```

As you can see, they do not take any arguments and return an object of type `pid_t`.

According to the POSIX standard, both functions __always succeed__.

### Exercise

Write a program that creates 'n' sub-processes (n is 1st program parameter), each of those processes waits for random [5-10]s time then prints its PID and terminates. Parent process prints the number of alive child processes every 3s. For now, do not worry about waiting for child processes.

### Solution

New man pages
- man 3p fork
- man 3p getpid
- man 3p wait
- man 3p waitpid
- man 3p sleep
- <a href="https://www.gnu.org/software/libc/manual/html_node/Job-Control.html">Job Control</a>

<em>solution <b>prog13a.c</b>:</em>
{{< includecode "prog13a.c" >}}

### Notes and questions 

- Make sure you know how the process group is created by shell, what processes belong to it?

- Please note that macro ERR was extended with kill(0, SIGKILL), it is meant to terminate the whole program (all other
processes) in case of error.

- Provide zero as pid argument of kill and you can send a signal to all the processes in the group. It is very useful not
to keep the PID's list in your program.

- Please notice that we do not test for errors inside of ERR macro (during error reporting), it is so to keep the program
action at minimal level at emergency exit. What else can we do ? Call ERR recursively and have the same errors again?

- Why after you run this program the command line returns immediately while processes are still working?
{{< details "Answer" >}} Parent process is not waiting for child processes, no wait or waitpid call. It will be fixed in the 2nd stage. {{< /details >}}

- How to check the current parent of the created sub-processes (after the initial parent quits)? Why this process?
{{< details "Answer" >}}  Right after the command line returns run: $ps -f, you should see that the PPID (parent PID) is 1 (init/systemd). It is caused by premature end of parent process, the orphaned processes can not "hang" outside of process three so they have to be attached somewhere. To make it simple, it is not the shell but the first process in the system. {{< /details >}}

- Random number generator seed is set in child process, can it be moved to parent process? Will it affect the program?
{{< details "Answer" >}} Child processes will get the same "random" numbers because they will have the same random seed. Seeding can not be moved to parent. {{< /details >}}

- Can we change the seed from PID to time() call?
{{< details "Answer" >}} No. Time you get from time() is returned in seconds since 1970, in most cases all sub-processes  will have the same seed and will get the same (not random) numbers. {{< /details >}}

- Try to derive a formula to get random number from the range [A,B], it should be obvious.

- How this program works if you remove the exit call in child code (right after child_work call)?
{{< details "Answer" >}} Child process after exiting the child_work will continue back into forking loop! It will start it's own children. Grandchildren can start their children and so on. To mess it up a bit more child processes do not wait for their children.  {{< /details >}}

- How many processes will be started in above case if you supply 3 as starting parameter?
{{< details "Answer" >}}  1 parent 3 children,  3 grand children and 1 grand grand child, 8 in total, draw a process three for it, tag the branches with current (on fork) n value. {{< /details >}}

- What sleep returns? Should we react to this value somehow?
{{< details "Answer" >}} It returns the time left to sleep at the moment of interruption bu signal handling function. In this code child processes does not receive nor handle the signals so this interruption is not possible.  In other codes it may be vital to restart sleep with remaining time. {{< /details >}}

- In the next stage child waiting and child counting will be added. How can we know how many child processes have exited?
{{< details "Answer" >}} SIGCHLD counting will not be precise as signals can marge, the only sure method is to count successful calls to wait or waitpid. {{< /details >}}

### Waiting for Child Processes
To prevent any leaks, before the parent process terminates, we must wait for all child processes to finish. We can do this using the `wait` function, which waits for any child process, or the `waitpid` function, which allows specifying which processes to wait for.  
Let’s look at their definitions:

```
pid_t wait(int *stat_loc);
pid_t waitpid(pid_t pid, int *stat_loc, int options);
```


As we can see, both functions return a `pid_t` object, which is the identifier of the process whose status we receive information about.

Both functions take the `stat_loc` argument of type `int*`, which points to a memory location where the information about the process’s status will be stored (if we do not need this information, we can pass `NULL`).

The `waitpid` function has two additional arguments: `pid` of type `pid_t` and `options` of type `int`.

The `pid` argument specifies which processes we want to wait for. Depending on its value, the function behaves as follows:  
- `pid == -1` - wait for any child process.
- `pid > 0` - wait for the process with the identifier equal to `pid`.
- `pid == 0` - wait for any process whose group ID is equal to the group ID of the calling process.
- `pid < -1` - wait for any process whose group ID is equal to the absolute value of `pid`.

The `options` argument specifies modifications to the function’s behavior and can be a combination of the following options:  
- `WCONTINUED` - the function should also return information about processes that were resumed after being stopped.
- `WNOHANG` - the `waitpid` function should not block the calling process if none of the processes we are waiting for can immediately report their status. In this case, the function returns `0`. 
- `WUNTRACED` - the function should also return information about processes that were stopped.

For the purposes of the lab, it is enough to know the `WNOHANG` option.

In summary, we can consider the `waitpid` function as a more advanced version of the `wait` function: calling `wait(stat_loc)` is equivalent to calling `waitpid(-1, stat_loc, 0)`.

Of course, both functions may fail, in which case they return `-1` and set the appropriate `errno` value.

__Note:__ If we call `wait` or `waitpid` and the pool of child processes to wait for is empty, the function returns `-1` and sets `errno` to `ECHILD`. It is useful to handle this to ensure that no processes are left orphaned before the parent process terminates.

### Exercise

Extend the program from the previous exercise to correctly wait for child processes.

### Solution

<em>solution <b>prog13b.c</b>:</em>
{{< includecode "prog13b.c" >}}

### Notes and questions

- It is worth knowing that waitpid can tell us about temporary lack of terminated children (returns zero) and about permanent lack of them (error ECHILD). The second case is not a critical error, your code should expect it.

- Why waitpid is in a loop?
{{< details "Answer" >}} we do not know how many zombie processes are there to collect,  it can be from zero to n of them. {{< /details >}}

- Why waitpid has the WNOHANG flag on?
{{< details "Answer" >}} we do not want to wait for alive child processes as we have to report the counter every 3 sec. to the user {{< /details >}}

- Why zero in place of pid in waitpid call?
{{< details "Answer" >}} We want to wait for any child process, we do not need to know children pids, zero means any of them. {{< /details >}}

- Does this program encounter signals? 
{{< details "Answer" >}} Yes - SIGCHILD. there is no handling routine but in this case it's alright, children are handled promptly by the above loop. {{< /details >}}

- Shouldn't we check sleep return value as we have signals in this code?
{{< details "Answer" >}} No, as we do not handle them. {{< /details >}}

## Task 2 - signals

Goal:
Program takes 4 positional parameters (n,k,p i r) and creates n sub-processes. Parent process sends sequentially SIGUSR1
and SIGUSR2 to all sub-processes in a loop with delays of k and p seconds (k sec. before SIGUSR1 and p sec. before
SIGUSR2). Parent process terminates after all its sub-processes. Each sub-process determines its own random time
delay [5,10] sec. and in a loop sleeps this time and prints SUCCESS on the stdout if the last signal received was
SIGUSR1 or FAILURE if it was SIGUSER2. This loop iterates r times.

What you need to know:
- man 7 signal
- man 3p sigaction
- man 3p nanosleep
- man 3p alarm
- man 3p memset
- man 3p kill

<em>solution <b>prog14.c</b>:</em>
{{< highlight c >}}
volatile sig_atomic_t last_signal = 0;

void sethandler( void (*f)(int), int sigNo) {
        struct sigaction act;
        memset(&act, 0, sizeof(struct sigaction));
        act.sa_handler = f;
        if (-1==sigaction(sigNo, &act, NULL)) ERR("sigaction");
}

void sig_handler(int sig) {
        printf("[%d] received signal %d\n", getpid(), sig);
        last_signal = sig;
}

void sigchld_handler(int sig) {
        pid_t pid;
        for(;;){
                pid=waitpid(0, NULL, WNOHANG);
                if(pid==0) return;
                if(pid<=0) {
                        if(errno==ECHILD) return;
                        ERR("waitpid");
                }
        }
}
{{< / highlight >}}

To exchange the data between signal handling routine and the main code we must use global variables, please remember
that it is an exceptional situation as in general we do not use global variables. Also please remember that global
variables are not shared between related processes. It should be obvious but sometimes students get confused.

The type of global variable used for this communication is fixed to be volatile sig_atomic_t, it is the only CORRECT and
SAFE type you can use here. The reason for this originates from asynchronous nature of the interruption. Primo "
volatile" means that compiler optimization is off, it is critical to not let the compiler eradicate the variable that is
not changing in the loop from the loop condition. With the optimization on while(work) may become while(1) as the
compiler in unable to determine the asynchronous change of "work" variable. Secundo sig_atomic_t is the biggest integer
variable that can be calculated and compared in single CPU cycle. If you try bigger integer simple comparison a==0 can
get interrupted and already compared bits may be altered during the comparison!

As you can see not much data can be exchanged between the handling function and the rest of the code, only simple
integers/states. Additionally we should not interrupt the main code for long. Putting that all together, we should
always keep the most of program logic in the main code, do not put much in the signal functions, it should be as short
as possible, only very simple expressions, assignment, increments and alike.

memset used to initialize the structures is quite often useful, especially if you do not know all the members of the
structure (quite often only part of members is described in man page, internally used members are unknown).

SIGCHLD handling function has a very similar code to what you have seen in first stage.

Do we expect more than one terminated child during SIGCHLD handling?
{{< details "Answer" >}} Yes, signals can merge, another child can terminate at the very moment of signal handling. {{< /details >}}

Do we expect zero terminated children at this handler? See ahead at the end of main.
{{< details "Answer" >}} Yes,  wait at the end of main can catch the child before SIGCHLD function does, then the function is left with zero children.It is a race condition.  {{< /details >}}

<em>solution <b>prog14.c</b>:</em>
{{< includecode "prog14.c" >}}

Please notice that sleep and alarm function can conflict, according to POSIX sleep can be implemented on SIGALRM and
there is no way to nest signals. Never nest them or use nanosleep instead of sleep as in the code above.

Kill function is invoked with zero pid, means it is sending signal to whole group of processes, we do not need to keep
track of pids but do notice that the signal will be delivered to the sender as well!

The location of setting of signal handling and blocking is not trivial, please analyze the example and answer the
questions below. Always plan in advance the reactions to signals in your program, this is a common student mistake to
overlook the problem.

Why sleep is in a loop, can the sleep time be exact in this case?
{{< details "Answer" >}} It gets interrupted by signal hadling, restart is a must. Sleep returns the remaining time rounded up to seconds so it can not be precise. {{< /details >}}

What is default  disposition of most of the signals (incl. SIGUSR1 and 2)?
{{< details "Answer" >}} Most not handled signals will kill the receiver. In this example the lack of handling, ignoring  or blocking of SIGUSR1 and 2 would kill the children. {{< /details >}}

How sending of SIGUSR1 and 2 to the process group affects the program?
{{< details "Answer" >}} Parent process has to be immune to them, the simplest solution is to ignore them. {{< /details >}}

What would happen if you turn this ignoring off?
{{< details "Answer" >}} Parent would kill itself with first signal sent. {{< /details >}} 

Can we shift the signal ignoring setup past the create_children? Child processes set their own signal disposition right at the start, do they need this ignoring?
{{< details "Answer" >}} They do need it, if you shift the setup and there is no ignoring inherited from the parent process it may happen (rare case but possible) that child process gets created but didn't start its code yet. Immediately after the creation, CPU slice goes to the parent that continues its code and sends the SIGUSR1 signal to the children. If then CPU slice goes back to the child, signal default disposition will kill it before it has a chance to set up its own handler! {{< /details >}}

Can we modify this program to avoid ignoring in the code?
{{< details "Answer" >}} In this program both child and a parent can have the same signal handling routines for SIGUSR1 and 2, you can set it just before fork and it will solve the problem. {{< /details >}}

Would shifting the setup of SIGCHLD handler past the fork change the program? 
{{< details "Answer" >}} If one of offspring "dies" very quickly (before parent sets its SIGCHLD handler) it will be a zombi until another offspring terminates. It is not a mayor mistake but it's worth attention. {{< /details >}}

Is wait call at the end of parent really needed? Parent waits long enough for children to finish, right?
{{< details "Answer" >}} Calculated time may not suffice, in overloaded system expect lags of any duration (few seconds and more), without "wait" children can terminate after the parent because of those lags. {{< /details >}}

## Waiting for a Signal

Often, when writing programs, we encounter a situation where a process, before performing its work, must be informed that another process has completed its task.  
As you might guess, this problem can be easily solved using __signals__. Inspired by the previous task, we could write logic where our process sleeps in a loop and checks whether the last signal it received is the one it is waiting for. Unfortunately, not only is this solution inelegant, but it is also __incorrect__, it could happen that the signal we are waiting for gets "merged" with another signal, and we would never actually know that our process can start its work.  

Fortunately, the operating system provides tools to solve this problem.

To block the program until it receives a signal, we use the `sigsuspend` function. Let’s look at its definition:

```
int sigsuspend(const sigset_t *sigmask);
```

As we can see, this function returns an `int` value, which is used to report a potential error (in that case, it returns `-1`). It takes an argument `sigmask` of type `const sigset_t *`, which is a pointer to a set of signals that the function will wait for.  

The function works as follows: it sets the signal mask to the one provided in the argument, waits to catch one of these signals, then restores the previous signal mask and resumes the execution of the process.

### Managing the Signal Mask

A set of signals is called a signal mask. We will store the signal mask as an object of type `sigset_t`. The standard does not specify how this type should be implemented; it can be either an `int` or a structure.  

To modify the signal mask, we use the functions `sigemptyset`, which initializes the mask as an empty set, and `sigaddset`, which adds a signal to the mask.  
Let’s look at their definitions:

```
int sigemptyset(sigset_t *set);
int sigaddset(sigset_t *set, int signo);
```

As we can see, both functions take `set` of type `sigset_t *` as the first argument, which is a pointer to the mask we want to edit.  
The function `sigaddset` additionally takes the `signo` argument, which is the code of the signal that, we want to add to the mask.

Both functions return an `int` value, used to indicate the operation's outcome: on success, they return `0`, and on error, they return `-1` and set the appropriate value of the `errno` variable.

### Changing the Signal Mask

Once we have defined a new signal mask, we want it to affect the operation of our process.  
For this purpose, we use the `sigprocmask` function, which determines how the signal mask we defined should affect the current signal mask of the process.  
Let’s look at its definition:
```
int sigprocmask(int how, const sigset_t *restrict set, sigset_t *restrict oset);
```

As we can see, this function takes the following arguments:  
- `how` of type `int` specifies how the new mask should affect the current mask. Available options are:  
  - `SIG_BLOCK` - the resulting signal mask is the union of the mask pointed to by `set` and the current signal mask (we specify which signals we want to __add__ to the mask).  
  - `SIG_SETMASK` - the resulting signal mask is the signal mask pointed to by `set`.  
  - `SIG_UNBLOCK` - the resulting signal mask is the intersection of the current mask and the complement of the mask pointed to by `set` (we specify which signals we want to __remove__ from the mask).  

- `set` of type `const sigset_t *` is a pointer to the mask we want to use to modify the previous mask.  
- `oset` of type `sigset_t *` is a pointer to an object where we want to save the signal mask before modification.


### Exercise

Write a program that starts one child process, which sends every "m" (parameter) microseconds a SIGUSR1 signal to the parent. Every
n-th signal is changed to SIGUSR2. Parent anticipates SIGUSR2 and counts the amount of signals received. Child process
also counts the amount of SIGUSR2 sent. Both processes print out the counted amounts at each signal operation. We reuse
some functions from previous code.

### Solution

New man pages:
- man 3p sigsuspend
- Glibc signal waiting<a href="http://www.gnu.org/software/libc/manual/html_node/Waiting-for-a-Signal.html#Waiting-for-a-Signal"> here</a>
- man 3p pthread_sigmask (sigprocmask only)
- man 3p sigaddset
- man 3p sigemptyset

<em>solution part <b>prog15.c</b>:</em>
{{< includecode "prog15.c" >}}

The program terminates on SIGINT (C-c)

### Notes and questions 

- Try it with various parameters. The shorter microsecond brake and more frequent SIGUSER2 the faster growing gap between
counters should be observable. In a moment the difference in numbers will be explained. If you do not observe the shift
between counters let the program run a bit longer - 1 minute should do.

- This code was written to show and explain certain problems, it can be easily improved, please keep this in mind when
reusing the code!

- Please do remember about getppid function. I have seen students programs passing parent pid as a parameter to the child
process function.

- Waiting for the signal with sigsuspend is a very common technique you must know. It is very well explained on GNU page
linked above. The rule of the thumb is to block the anticipated signal first and for most of the program time. It gets
unblocked at the moment program can wait - at sigsuspend call. Now the signal can influence our main code only in well
defined points when it is not processing. It is a great advantage for us to limit the signals to certain moments only.

- When above method is in use you can stop worrying about asynchronous codes, they are now synchronous and you can use
more data types for communication via globals and have longer signal handlers.

- Which counter gets skewed? Parent's or child's?
{{< details "Answer" >}} It must be the slower one, program can not count not sent signals, it can only lose some. Only the receiver can miss some of the signal thus the problem is in the parent process. {{< /details >}}

- Why counters are shifted?
{{< details "Answer" >}} You probably blame signal merging but it has small chance to make any impact. The source of the problem is within sigsuspend as THERE IS NO GUARANTEE THAT DURING ONE CALL TO IT ONLY ONE SIGNAL WILL BE HANDLED! It is a very common misconception! Right after program executes the handler for SIGUSR2 in the duration of the same sigsuspend it executes the handler for SIGUSR1, global variable gets overwritten and parent process has no chance to count the SIGUSR2!   {{< /details >}}

- How can we run the program to lower SIGUSR2 merging chances to zero and still observe skewed counter?
{{< details "Answer" >}} Run with short brakes between signals and lots of SIGUSR1 between SIGUSR2. Now SIGUSR2 are very unlikely to merge as signals are separated in time by a lot of SIGUSR1, short brakes between signals rises the chance to have multiple handlers run in one sigsuspend. {{< /details >}}

- Correct the above program to eliminate the above problem.
{{< details "Answer" >}} You can have a dedicated global variable only for SIGUSR2, increasing of the counter of SIGUSR2 can run in handler itself it will eliminate the problem of multiple SIGUSR2 handler call in one sigsuspend. Modify the counter printout and it is ready. {{< /details >}}

## Task 4 - low level file access and signals

Goal:
Modify task 3 code. Parent receives SIGUSR1 form child at set interval (1st parameter) and counts them. Additionally parent process creates a file of set (2nd parameter) amount of blocks of set size (3rd parameter) with a name given as 4th parameter. The content of the file is a copy of data read from /dev/urandom. Each block must be copied separately with sizes control. After each copy operation program prints the effective amount of data transferred and the amount of received signals on the stderr.
<em>What you need to know:</em> 
- man 4 urandom

{{< hint info >}}
This task has two stages.
{{< /hint >}}

<em>solution 1st stage, parts of <b>prog16a.c</b>:</em>
{{< includecode "prog16a.c" >}}

Do remember that you can read good quality really random bytes from /dev/random file but the amount is limited or read
unlimited amount of data from /dev/urandom but these are pseudo random bytes.

You should see the following flaws if you run the program with 1 20 40 out.txt params:

Coping of blocks shorter than 40Mb, in my case it was at most 33554431, it is due to signal interruption DURING the IO
operation

fprintf: Interrupted system call - function interrupted by signal handling BEFORE it did anything

Similar messages for open and close - it may be hard to observe in this program but it is possible and described by
POSIX documentation.

How to get rid of those flows is explained in the 2nd stage.

If there is a memory allocation in your code, there also HAS to be a memory release! Always.

Permissions passed to open function can also be expressed with predefined constants (man 3p mknod). As octal permission
representation is well recognized by programmers and administrators it can also be noted in this way and will not be
considered as "magic number" style mistake. It is fairly easy to trace those constants in the code.

Obviously the parent counts less signals than child sends, as summing runs inside the handler we can only blame merging for it. Can you tell why signal merging is so strong in this code?
{{< details "Answer" >}} In this architecture (GNU/Linux) CPU planer blocks signals during IO operations (to some size as we can see) and  during IO signals have more time to merge. {{< /details >}}

What for the SIGUSR1 is sent to the process group at the end of the parent process?
{{< details "Answer" >}} To terminate the child. {{< /details >}}

How come it works? SIGUSR1 handling is inherited from the parent?
{{< details "Answer" >}} Child first action is to restore default signal disposition - killing of the receiver. {{< /details >}}

Why parent does not kill itself with this signal?
{{< details "Answer" >}} It sets the handler for SIGUSR1 before it sends it to the group. {{< /details >}}

Can this strategy fail?
{{< details "Answer" >}} Yes, if parent process finishes it's job before child is able to even start the code and reset SIGUSR1 disposition. {{< /details >}}

Can you improve it and at the same time not kill the parent with the signal from a child?
{{< details "Answer" >}} send SIGUSR2 to the child. {{< /details >}} 

Is this child (children) termination strategy  easy and correct at the same time in all possible programs?
{{< details "Answer" >}} Only if child processe does not have resources to release, if it has something to release you must add proper signal handling and this may be complicated. {{< /details >}}

Why to check if a pointer to newly allocated memory is not null?
{{< details "Answer" >}} Operating system may not be able to grant your program additional memory, in this case it reports the error with the NULL. You must be prepared for it. The lack of this check is a common students' mistake. {{< /details >}}

Can you turn the allocated buffer into automatic variable and avoid the effort of allocating and releasing the memory?
{{< details "Answer" >}} I don't know about OS architecture that uses stacks large enough to accommodate 40MB, typical stack has a few MB at most. For smaller buffers (a few KB) it can work. {{< /details >}}  

Why permissions of a newly created file are supposed to be full (0777)? Are they really full?
{{< details "Answer" >}} umask will reduce the permissions, if no set  permissions are required it is a good idea to allow the umask to regulate the effective rights {{< /details >}}

<em>solution 2nd stage, parts of <b>prog16b.c</b>:</em>
{{< includecode "prog16b.c" >}}

Run it with the same parameters as before - flaws are gone now.

What error code  EINTR represents?
{{< details "Answer" >}} This is not an error, it is a way for OS to inform the program that the signal handler has been invoked {{< /details >}}

How should you react to EINTR?
{{< details "Answer" >}} Unlike real errors do not exit the program, in most cases  to recover the problem simply restart the interrupted function with the same set of parameters as in initial call. {{< /details >}}

At what stage functions are interrupted if EINTR is reported
{{< details "Answer" >}} Only before they start doing their job - in waiting stage. This means that you can safely restart with the same arguments all the functions used in OPS tutorials except "connect" (OPS2 sockets) {{< /details >}} 

What are other types of interruption signal handler can cause?
{{< details "Answer" >}} IO transfer can be interrupted in the middle, this case is not reported with EINTR. Sleep and nanosleep similar. In both cases restarting can not reuse the same parameters, it gets complicated. {{< /details >}}

How do you know what function cat report EINTR?
{{< details "Answer" >}}  Read man pages, error sections. It easy to guess those function must wait before they do their job. {{< /details >}}

Analyze how bulk_read and bulk_write work. You should know what cases are recognized in those functions, what types of interruption they can handle, how to recognize EOF on the descriptor.
Unlike during L1 lab, during L2 and following labs you have to use these functions (or similar ones) when calling `read` or `write` (because we use signals now).
If you do not use them, you wont get points for your solution.

Both bulk_ functions can be useful not only on signals but also to "glue" IO transfers where data comes from not
continuous data sources like pipe/fifo and the socket - it wile be covered by following tutorials.

Not only read/write can be interrupted in the described way, the problem applies to the related pairs like fread/fwrite
and send/recv.

As you know SA_RESTART flag can cause automatic restarts on delivery of a signal if this flag is set in the handler, it
may not be apparent but this method has a lot of shortcomings:

You must control all the signal handlers used in the code, they all must be set with this flag, if one does not use this
flag then you must handle EINTR as usual. It is easy to forget about this requirement if you extend/reuse the older
code.

If you try to make some library functions (like bulk_read and write) you can not assume anything about the signals in
the caller code.

It is hard to reuse a code depending on SA_RESTART flag, it can only be transferred to the similar strict handler
control code.

Sometimes you wish to know about interruption ASAP to react quickly. Sigsuspend would not work if you use this flag!

Why do we not react on other (apart from EINTR) errors of fprintf? If program can not write on stderr (most likely
screen) then it cannot report errors.

Really big (f)printfs can get interrupted in the middle of the process (like write). Then it is difficult to restart the
process especially if formatting is complicated. Avoid using printf where restarting would be critical (most cases
except for the screen output) and the volume of transferred data is significant, use write instead.

Do the example tasks. During the laboratory you will have more time and a starting code. If you do following tasks in the allotted time, it means that you are well-prepared.

- [Task 1]({{< ref "/sop1/lab/l2/example1" >}}) ~75 minutes
- [Task 2]({{< ref "/sop1/lab/l2/example2" >}}) ~120 minutes
- [Task 3]({{< ref "/sop1/lab/l2/example3" >}}) ~120 minutes

## Source codes presented in this tutorial

{{% codeattachments %}}
