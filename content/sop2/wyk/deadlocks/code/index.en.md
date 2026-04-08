---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

### Debugging with GDB

```shell
make dining_philosophers
./dining_philosophers
```

Option 1) Run process under GDB

```shell
gdb ./dining_philosophers
```

Use `r` command to start the execution, then `C-c` to exit to prompt once program hangs.
Use `bt` and `thread apply all bt` to analyze where all threads are hanged.
Use `q` to quit GDB.

Option 2) Attach to a running process

```shell
# Start independent process
./dining_philosophers
```

```shell
# Attach GDB to a running, deadlocked process
gdb -p $(pidof dining_philosophers)
```

You may need to run `sudo sysctl kernel.yama.ptrace_scope=0`.

Use `thread apply all bt` to analyze where all threads are hanged.

Option 3) Create and debug a coredump file

```shell
# Generate a coredump of a running process with gcore
gcore -o dining_philosophers.core $(pidof dining_philosophers)
ls -lah dining_philosophers.core.$(pidof dining_philosophers)
```

```shell
gdb /tmp/dining_philosophers -c dining_philosophers.core.$(pidof dining_philosophers)
```

Try also debugging a coredump file using your IDE!

