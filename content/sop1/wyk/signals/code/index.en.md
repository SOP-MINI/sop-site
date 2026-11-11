---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

From `man 7 signal`:

```text
Signal      Standard   Action   Comment
────────────────────────────────────────────────────────────────────────
SIGABRT      P1990      Core    Abort signal from abort(3)
SIGALRM      P1990      Term    Timer signal from alarm(2)
SIGBUS       P2001      Core    Bus error (bad memory access)
SIGCHLD      P1990      Ign     Child stopped or terminated
SIGCLD         -        Ign     A synonym for SIGCHLD
SIGCONT      P1990      Cont    Continue if stopped
SIGEMT         -        Term    Emulator trap
SIGFPE       P1990      Core    Floating-point exception
SIGHUP       P1990      Term    Hangup detected on controlling terminal
                                or death of controlling process
SIGILL       P1990      Core    Illegal Instruction
SIGINFO        -                A synonym for SIGPWR
SIGINT       P1990      Term    Interrupt from keyboard
SIGIO          -        Term    I/O now possible (4.2BSD)
SIGIOT         -        Core    IOT trap. A synonym for SIGABRT
SIGKILL      P1990      Term    Kill signal
SIGLOST        -        Term    File lock lost (unused)
SIGPIPE      P1990      Term    Broken pipe: write to pipe with no
                                readers; see pipe(7)
SIGPOLL      P2001      Term    Pollable event (Sys V).
                                Synonym for SIGIO
SIGPROF      P2001      Term    Profiling timer expired
SIGPWR         -        Term    Power failure (System V)
SIGQUIT      P1990      Core    Quit from keyboard
SIGSEGV      P1990      Core    Invalid memory reference
SIGSTKFLT      -        Term    Stack fault on coprocessor (unused)
SIGSTOP      P1990      Stop    Stop process
SIGTSTP      P1990      Stop    Stop typed at terminal
SIGSYS       P2001      Core    Bad system call (SVr4);
                                see also seccomp(2)
SIGTERM      P1990      Term    Termination signal
SIGTRAP      P2001      Core    Trace/breakpoint trap
SIGTTIN      P1990      Stop    Terminal input for background process
SIGTTOU      P1990      Stop    Terminal output for background process
SIGUNUSED      -        Core    Synonymous with SIGSYS
SIGURG       P2001      Ign     Urgent condition on socket (4.2BSD)
SIGUSR1      P1990      Term    User-defined signal 1
SIGUSR2      P1990      Term    User-defined signal 2

SIGVTALRM    P2001      Term    Virtual alarm clock (4.2BSD)
SIGXCPU      P2001      Core    CPU time limit exceeded (4.2BSD);
                                see setrlimit(2)
SIGXFSZ      P2001      Core    File size limit exceeded (4.2BSD);
                                see setrlimit(2)
SIGWINCH       -        Ign     Window resize signal (4.3BSD, Sun)
```

```text
The signals SIGKILL and SIGSTOP cannot be caught, blocked, or ignored.
```

## The kill() syscall

```shell
make kill
./kill
```
Source: [kill.c]({{< github_url "kill.c" >}})

Try killing:
* One of the children
* Invalid PID
* Whole process group (-PGID)
* PID -1

## Signal action

Signal lifecycle: generation -> pending -> delivery/acceptance

Signals can be:

* **delivered**
  * ignored
  * default handler
  * user-defined handler
* **accepted**

## User-defined handlers

Note: Signal handlers execute asynchronously and may only call async-signal-safe functions (see `man 7 signal-safety`), such as `write()` instead of `printf()` or `perror()`. If a handler calls functions that may modify `errno` (e.g. `waitpid()`, `read()`, `write()`), it should save `errno` on entry and restore it before returning.

```shell
make basic_handler
./basic_handler
```
Source: [basic_handler.c]({{< github_url "basic_handler.c" >}})

Try pressing C-c and using the `kill` command.

Try also setting the handler to `SIG_IGN` and `SIG_DFL`.

Unblock fork to demonstrate the handler inheritance.

```shell
make segv_handler
./segv_handler
```
Source: [segv_handler.c]({{< github_url "segv_handler.c" >}})

Don't do that at home.

```shell
make handler
./handler
```
Source: [handler.c]({{< github_url "handler.c" >}})

Why program isn't getting killed on typing 0 or -PGID?

```shell
make long_handler
./long_handler
```
Source: [long_handler.c]({{< github_url "long_handler.c" >}})

Observe signal blocking for the time spent within the handler.

## Blocking signals

```shell
make basic_blocking
./basic_blocking
```
Source: [basic_blocking.c]({{< github_url "basic_blocking.c" >}})

```shell
make blocking
./blocking
```
Source: [blocking.c]({{< github_url "blocking.c" >}})

Observe contents of proc during execution:

```shell
watch -n0.1 "cat /proc/<pid>/status | grep -E '(Sig|Shd)'"
```

## Meaning of `EINTR`

```shell
make eintr
./eintr
```
Source: [eintr.c]({{< github_url "eintr.c" >}})

Try adding `SA_RESTART` flag to sigchld_handler setup.

```shell
make partial_read
./partial_read /dev/stdin
```
Source: [partial_read.c]({{< github_url "partial_read.c" >}})

## Usage of `volatile sig_atomic_t`

```shell
make sigatomic
./sigatomic
```
Source: [sigatomic.c]({{< github_url "sigatomic.c" >}})

## `SIGCHLD` signal

```shell
make sigchld
./sigchld
```
Source: [sigchld.c]({{< github_url "sigchld.c" >}})

## Synchronous handling

```shell
make sigsuspend
./sigsuspend
```
Source: [sigsuspend.c]({{< github_url "sigsuspend.c" >}})

```shell
make sigwait
./sigwait
```
Source: [sigwait.c]({{< github_url "sigwait.c" >}})

## Complete example

```shell
make lab_example
./lab_example 3
```
Source: [lab_example.c]({{< github_url "lab_example.c" >}})
