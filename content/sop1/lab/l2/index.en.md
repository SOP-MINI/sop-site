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
- Quite often you will find $ sign placed before commands you should run in the shell, obviously you do not need to rewrite this sight to command line, I put it there to remind you that it is a command to execute.
- What you learn and practice in this tutorial will be required for the next ones. If you have a problem with this material after the graded lab you can still ask teachers for help. 
- This time some of the solutions are divided into two stages
{{< /hint >}}

## Task 1 - processes

Goal: 
Program creates 'n' sub-processes (n is 1st program parameter), each of those processes waits for random [5-10]s time then prints its PID and terminates. Parent process prints the number of alive child processes every 3s.
<em>What you need to know:</em> 
- man 3p fork
- man 3p getpid
- man 3p wait
- man 3p waitpid
- man 3p sleep
- <a href="https://www.gnu.org/software/libc/manual/html_node/Job-Control.html">Job Control</a>

<em>solution, 1st stage <b>prog13a.c</b>:</em>
{{< includecode "prog13a.c" >}}

Use the general makefile (the last one) from the first tutorial, execute "make prog13a"

Make sure you know how the process group is created by shell, what processes belong to it?

Please note that macro ERR was extended with kill(0, SIGKILL), it is meant to terminate the whole program (all other
processes) in case of error.

Provide zero as pid argument of kill and you can send a signal to all the processes in the group. It is very useful not
to keep the PID's list in your program.

Please notice that we do not test for errors inside of ERR macro (during error reporting), it is so to keep the program
action at minimal level at emergency exit. What else can we do ? Call ERR recursively and have the same errors again?

Why after you run this program the command line returns immediately while processes are still working?
{{< expand "Answer" >}} Parent process is not waiting for child processes, no wait or waitpid call. It will be fixed in the 2nd stage. {{< /expand >}}

How to check the current parent of the created sub-processes (after the initial parent quits)? Why this process?
{{< expand "Answer" >}}  Right after the command line returns run: $ps -f, you should see that the PPID (parent PID) is 1 (init/systemd). It is caused by premature end of parent process, the orphaned processes can not "hang" outside of process three so they have to be attached somewhere. To make it simple, it is not the shell but the first process in the system. {{< /expand >}}

Random number generator seed is set in child process, can it be moved to parent process? Will it affect the program?
{{< expand "Answer" >}} Child processes will get the same "random" numbers because they will have the same random seed. Seeding can not be moved to parent. {{< /expand >}}

Can we change the seed from PID to time() call?
{{< expand "Answer" >}} No. Time you get from time() is returned in seconds since 1970, in most cases all sub-processes  will have the same seed and will get the same (not random) numbers. {{< /expand >}}

Try to derive a formula to get random number from the range [A,B], it should be obvious.

How this program works if you remove the exit call in child code (right after child_work call)?
{{< expand "Answer" >}} Child process after exiting the child_work will continue back into forking loop! It will start it's own children. Grandchildren can start their children and so on. To mess it up a bit more child processes do not wait for their children.  {{< /expand >}}

How many processes will be started in above case if you supply 3 as starting parameter?
{{< expand "Answer" >}}  1 parent 3 children,  3 grand children and 1 grand grand child, 8 in total, draw a process three for it, tag the branches with current (on fork) n value. {{< /expand >}}

What sleep returns? Should we react to this value somehow?
{{< expand "Answer" >}} It returns the time left to sleep at the moment of interruption bu signal handling function. In this code child processes does not receive nor handle the signals so this interruption is not possible.  In other codes it may be vital to restart sleep with remaining time. {{< /expand >}}

In the next stage child waiting and child counting will be added. How can we know how many child processes have exited?
{{< expand "Answer" >}} SIGCHLD counting will not be precise as signals can marge, the only sure method is to count successful calls to wait or waitpid. {{< /expand >}}

<em>solution 2nd stage <b>prog13b.c</b>:</em>
{{< includecode "prog13b.c" >}}

It is worth knowing that waitpid can tell us about temporary lack of terminated children (returns zero) and about permanent lack of them (error ECHILD). The second case is not a critical error, your code should expect it.

Why waitpid is in a loop?
{{< expand "Answer" >}} we do not know how many zombie processes are there to collect,  it can be from zero to n of them. {{< /expand >}}

Why waitpid has the WNOHANG flag on?
{{< expand "Answer" >}} we do not want to wait for alive child processes as we have to report the counter every 3 sec. to the user {{< /expand >}}

Why zero in place of pid in waitpid call?
{{< expand "Answer" >}} We want to wait for any child process, we do not need to know children pids, zero means any of them. {{< /expand >}}

Does this program encounter signals? 
{{< expand "Answer" >}} Yes - SIGCHILD. there is no handling routine but in this case it's alright, children are handled promptly by the above loop. {{< /expand >}}

Shouldn't we check sleep return value as we have signals in this code?
{{< expand "Answer" >}} No, as we do not handle them. {{< /expand >}}

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
                if(pid&lt;=0) {
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
{{< expand "Answer" >}} Yes, signals can merge, another child can terminate at the very moment of signal handling. {{< /expand >}}

Do we expect zero terminated children at this handler? See ahead at the end of main.
{{< expand "Answer" >}} Yes,  wait at the end of main can catch the child before SIGCHLD function does, then the function is left with zero children.It is a race condition.  {{< /expand >}}

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
{{< expand "Answer" >}} It gets interrupted by signal hadling, restart is a must. Sleep returns the remaining time rounded up to seconds so it can not be precise. {{< /expand >}}

What is default  disposition of most of the signals (incl. SIGUSR1 and 2)?
{{< expand "Answer" >}} Most not handled signals will kill the receiver. In this example the lack of handling, ignoring  or blocking of SIGUSR1 and 2 would kill the children. {{< /expand >}}

How sending of SIGUSR1 and 2 to the process group affects the program?
{{< expand "Answer" >}} Parent process has to be immune to them, the simplest solution is to ignore them. {{< /expand >}}

What would happen if you turn this ignoring off?
{{< expand "Answer" >}} Parent would kill itself with first signal sent. {{< /expand >}} 

Can we shift the signal ignoring setup past the create_children? Child processes set their own signal disposition right at the start, do they need this ignoring?
{{< expand "Answer" >}} They do need it, if you shift the setup and there is no ignoring inherited from the parent process it may happen (rare case but possible) that child process gets created but didn't start its code yet. Immediately after the creation, CPU slice goes to the parent that continues its code and sends the SIGUSR1 signal to the children. If then CPU slice goes back to the child, signal default disposition will kill it before it has a chance to set up its own handler! {{< /expand >}}

Can we modify this program to avoid ignoring in the code?
{{< expand "Answer" >}} In this program both child and a parent can have the same signal handling routines for SIGUSR1 and 2, you can set it just before fork and it will solve the problem. {{< /expand >}}

Would shifting the setup of SIGCHLD handler past the fork change the program? 
{{< expand "Answer" >}} If one of offspring "dies" very quickly (before parent sets its SIGCHLD handler) it will be a zombi until another offspring terminates. It is not a mayor mistake but it's worth attention. {{< /expand >}}

Is wait call at the end of parent really needed? Parent waits long enough for children to finish, right?
{{< expand "Answer" >}} Calculated time may not suffice, in overloaded system expect lags of any duration (few seconds and more), without "wait" children can terminate after the parent because of those lags. {{< /expand >}}

## Task 3 - signal waiting

Goal:
Program starts one child process, which sends every "m" (parameter) microseconds a SIGUSR1 signal to the parent. Every
n-th signal is changed to SIGUSR2. Parent anticipates SIGUSR2 and counts the amount of signals received. Child process
also counts the amount of SIGUSR2 sent. Both processes print out the counted amounts at each signal operation. We reuse
some functions from previous code.

What you need to know:
- man 3p sigsuspend
- Glibc signal waiting<a href="http://www.gnu.org/software/libc/manual/html_node/Waiting-for-a-Signal.html#Waiting-for-a-Signal"> here</a>
- man 3p getppid 
- man 3p pthread_sigmask (sigprocmask only)
- man 3p sigaddset
- man 3p sigemptyset

<em>solution part <b>prog15.c</b>:</em>
{{< includecode "prog15.c" >}}

The program terminates on SIGINT (C-c)

Try it with various parameters. The shorter microsecond brake and more frequent SIGUSER2 the faster growing gap between
counters should be observable. In a moment the difference in numbers will be explained. If you do not observe the shift
between counters let the program run a bit longer - 1 minute should do.

This code was written to show and explain certain problems, it can be easily improved, please keep this in mind when
reusing the code!

Please do remember about getppid function. I have seen students programs passing parent pid as a parameter to the child
process function.

Waiting for the signal with sigsuspend is a very common technique you must know. It is very well explained on GNU page
linked above. The rule of the thumb is to block the anticipated signal first and for most of the program time. It gets
unblocked at the moment program can wait - at sigsuspend call. Now the signal can influence our main code only in well
defined points when it is not processing. It is a great advantage for us to limit the signals to certain moments only.

When above method is in use you can stop worrying about asynchronous codes, they are now synchronous and you can use
more data types for communication via globals and have longer signal handlers.

Which counter gets skewed? Parent's or child's?
{{< expand "Answer" >}} It must be the slower one, program can not count not sent signals, it can only lose some. Only the receiver can miss some of the signal thus the problem is in the parent process. {{< /expand >}}

Why counters are shifted?
{{< expand "Answer" >}} You probably blame signal merging but it has small chance to make any impact. The source of the problem is within sigsuspend as THERE IS NO GUARANTEE THAT DURING ONE CALL TO IT ONLY ONE SIGNAL WILL BE HANDLED! It is a very common misconception! Right after program executes the handler for SIGUSR2 in the duration of the same sigsuspend it executes the handler for SIGUSR1, global variable gets overwritten and parent process has no chance to count the SIGUSR2!   {{< /expand >}}

How can we run the program to lower SIGUSR2 merging chances to zero and still observe skewed counter?
{{< expand "Answer" >}} Run with short brakes between signals and lots of SIGUSR1 between SIGUSR2. Now SIGUSR2 are very unlikely to merge as signals are separated in time by a lot of SIGUSR1, short brakes between signals rises the chance to have multiple handlers run in one sigsuspend. {{< /expand >}}

Correct the above program to eliminate the above problem.
{{< expand "Answer" >}} You can have a dedicated global variable only for SIGUSR2, increasing of the counter of SIGUSR2 can run in handler itself it will eliminate the problem of multiple SIGUSR2 handler call in one sigsuspend. Modify the counter printout and it is ready. {{< /expand >}}

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
{{< expand "Answer" >}} In this architecture (GNU/Linux) CPU planer blocks signals during IO operations (to some size as we can see) and  during IO signals have more time to merge. {{< /expand >}}

What for the SIGUSR1 is sent to the process group at the end of the parent process?
{{< expand "Answer" >}} To terminate the child. {{< /expand >}}

How come it works? SIGUSR1 handling is inherited from the parent?
{{< expand "Answer" >}} Child first action is to restore default signal disposition - killing of the receiver. {{< /expand >}}

Why parent does not kill itself with this signal?
{{< expand "Answer" >}} It sets the handler for SIGUSR1 before it sends it to the group. {{< /expand >}}

Can this strategy fail?
{{< expand "Answer" >}} Yes, if parent process finishes it's job before child is able to even start the code and reset SIGUSR1 disposition. {{< /expand >}}

Can you improve it and at the same time not kill the parent with the signal from a child?
{{< expand "Answer" >}} send SIGUSR2 to the child. {{< /expand >}} 

Is this child (children) termination strategy  easy and correct at the same time in all possible programs?
{{< expand "Answer" >}} Only if child processe does not have resources to release, if it has something to release you must add proper signal handling and this may be complicated. {{< /expand >}}

Why to check if a pointer to newly allocated memory is not null?
{{< expand "Answer" >}} Operating system may not be able to grant your program additional memory, in this case it reports the error with the NULL. You must be prepared for it. The lack of this check is a common students' mistake. {{< /expand >}}

Can you turn the allocated buffer into automatic variable and avoid the effort of allocating and releasing the memory?
{{< expand "Answer" >}} I don't know about OS architecture that uses stacks large enough to accommodate 40MB, typical stack has a few MB at most. For smaller buffers (a few KB) it can work. {{< /expand >}}  

Why permissions of a newly created file are supposed to be full (0777)? Are they really full?
{{< expand "Answer" >}} umask will reduce the permissions, if no set  permissions are required it is a good idea to allow the umask to regulate the effective rights {{< /expand >}}

<em>solution 2nd stage, parts of <b>prog16b.c</b>:</em>
{{< includecode "prog16b.c" >}}

Run it with the same parameters as before - flaws are gone now.

For a program to see `TEMP_FAILURE_RETRY` macro you must first define `GNU_SOURCE` and then include header
file `unistd.h`.

What error code  EINTR represents?
{{< expand "Answer" >}} This is not an error, it is a way for OS to inform the program that the signal handler has been invoked {{< /expand >}}

How should you react to EINTR?
{{< expand "Answer" >}} Unlike real errors do not exit the program, in most cases  to recover the problem simply restart the interrupted function with the same set of parameters as in initial call. {{< /expand >}}

At what stage functions are interrupted if EINTR is reported
{{< expand "Answer" >}} Only before they start doing their job - in waiting stage. This means that you can safely restart with the same arguments all the functions used in OPS tutorials except "connect" (OPS2 sockets) {{< /expand >}} 

What are other types of interruption signal handler can cause?
{{< expand "Answer" >}} IO transfer can be interrupted in the middle, this case is not reported with EINTR. Sleep and nanosleep similar. In both cases restarting can not reuse the same parameters, it gets complicated. {{< /expand >}}

How do you know what function cat report EINTR?
{{< expand "Answer" >}}  Read man pages, error sections. It easy to guess those function must wait before they do their job. {{< /expand >}}

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

As an exercise do <a href="{{< ref "../l2-example" >}}">this</a> task. It was used in previous years in a bit different labs timing. It is 60 minutes task and if you can do it in this time it means you are prepared for the lab. In a new timing there is more time for the task and it will be slightly larger.

## Source codes presented in this tutorial

{{% codeattachments %}}
