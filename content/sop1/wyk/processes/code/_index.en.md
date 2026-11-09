---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

### Listing processes

```shell
ps -eo pid,state,command
```

From `man ps`:
```text
PROCESS STATE CODES
Here are the different values that the s, stat and state output specifiers
(header "STAT" or "S") will display to describe the state of a process:

D    uninterruptible sleep (usually IO)
I    Idle kernel thread
R    running or runnable (on run queue)
S    interruptible sleep (waiting for an event to complete)
T    stopped by job control signal
t    stopped by debugger during the tracing
W    paging (not valid since the 2.6.xx kernel)
X    dead (should never be seen)
Z    defunct ("zombie") process, terminated but not reaped by its parent
```
Try also listing contents of the `/proc` subdirectories.

### Linux task structure (PCB)

[struct task_struct](https://elixir.bootlin.com/linux/latest/source/include/linux/sched.h#L738)

Some of its members:
* [PID](https://elixir.bootlin.com/linux/v6.5.8/source/include/linux/sched.h#L962)
* [state](https://elixir.bootlin.com/linux/v6.5.8/source/include/linux/sched.h#L746)
  * [enum](https://elixir.bootlin.com/linux/v6.5.8/source/include/linux/sched.h#L86)
* [children](https://elixir.bootlin.com/linux/v6.5.8/source/include/linux/sched.h#L984)
* [exit code](https://elixir.bootlin.com/linux/v6.5.8/source/include/linux/sched.h#L875)
* [CPU state](https://elixir.bootlin.com/linux/v6.5.8/source/include/linux/sched.h#L1541)

### Virtual address space

```shell
make vm
./vm
```
Source: [vm.c]({{< github_url "vm.c" >}})

### Counting context switches

```shell
#gcc -O0 -o cs_count cs.c
make cs_count
perf stat -e cs ./cs_count
```
Source: [cs.c]({{< github_url "cs.c" >}})

Note this may silently fail with count = 0 if kernel options are not properly adjusted:

```shell
sudo sh -c 'echo 1 >/proc/sys/kernel/perf_event_paranoid'
sudo sh -c 'echo 0 >/proc/sys/kernel/kptr_restrict'
```

See also: [Debugging context switches](cs)

### Hello fork()

```shell
make hello
./hello
```
Source: [hello.c]({{< github_url "hello.c" >}})

### Descriptor inheritance

```shell
rm -Rf /tmp/fork_write.txt
make fork_write
./fork_write /tmp/fork_write.txt aaa bbbb
```
Source: [fork_write.c]({{< github_url "fork_write.c" >}})

```shell
watch -n0.1 cat /tmp/fork_write.txt
```

```shell
make concurrent_read
./concurrent_read
```
Source: [concurrent_read.c]({{< github_url "concurrent_read.c" >}})

### Waiting for children

```shell
make wait
./wait
```
Source: [wait.c]({{< github_url "wait.c" >}})

### Orphans

```shell
make orphan
./orphan
```
Source: [orphan.c]({{< github_url "orphan.c" >}})

### Zombies

```shell
make zombie
./zombie 3
```
Source: [zombie.c]({{< github_url "zombie.c" >}})

Run `ps -eo pid,state,cmd` during the program execution.

### Exec

```shell
make env
./env
```
Source: [env.c]({{< github_url "env.c" >}})

```shell
make exec
./exec
```
Source: [exec.c]({{< github_url "exec.c" >}})

### Sessions

```shell
make demon
./demon
tail -f /tmp/demon.log
```
Source: [demon.c]({{< github_url "demon.c" >}})

### clone() example

```shell
make clone
./clone
```
Source: [clone.c]({{< github_url "clone.c" >}})

### Process tree example

```shell
make lab
./lab 3
```
Source: [lab.c]({{< github_url "lab.c" >}})
