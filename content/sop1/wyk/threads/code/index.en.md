---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

First review general slides up to pthreads introduction.

### Basic examples

```shell
#gcc -o basic_thread -g basic_thread.c -pthread -fsanitize=address
make basic_thread
./basic_thread
```
Source: [basic_thread.c]({{< github_url "basic_thread.c" >}})

```shell
#gcc -DNOJOIN -o basic_thread_nojoin -g basic_thread.c -pthread
make basic_thread_nojoin
valgrind --leak-check=full --show-leak-kinds=all ./basic_thread_nojoin
```

```shell
make basic_thread_stack_version
./basic_thread_stack_version
```
Source: [basic_thread_stack_version.c]({{< github_url "basic_thread_stack_version.c" >}})

```shell
make thread_args
./thread_args
```
Source: [thread_args.c]({{< github_url "thread_args.c" >}})

```shell
#gcc -o join -g join.c -pthread -fsanitize=address
make join
./join
```
Source: [join.c]({{< github_url "join.c" >}})

### Detached threads

Review attribute slides.

```shell
make detach
./detach
```
Source: [detach.c]({{< github_url "detach.c" >}})

```shell
valgrind --leak-check=full --show-leak-kinds=all ./detach
```

### Cancellation

Review thread cancellation slides.

```shell
make cancel
./cancel
```
Source: [cancel.c]({{< github_url "cancel.c" >}})

```shell
#gcc -DNOCANCEL -o cancel_nocancel -g cancel.c -pthread
make cancel_nocancel
./cancel_nocancel
```

Review cleanup handlers slides.

```shell
make cleanup
./cleanup
```
Source: [cleanup.c]({{< github_url "cleanup.c" >}})

```shell
#gcc -DASYNC -o cleanup_async -g cleanup.c -pthread
make cleanup_async
./cleanup_async
```

### Concurrent data access

```shell
make concurrent_write
./concurrent_write
```
Source: [concurrent_write.c]({{< github_url "concurrent_write.c" >}})

Try using thread sanitizer:

```shell
#gcc -o concurrent_write_tsan -g concurrent_write.c -pthread -fsanitize=thread
make concurrent_write_tsan
./concurrent_write_tsan
```

Review mutex slides.

```shell
make mutex
./mutex
```
Source: [mutex.c]({{< github_url "mutex.c" >}})

### Threads and signals

Review signals slides.

```shell
make thread_signals
./thread_signals
```
Source: [thread_signals.c]({{< github_url "thread_signals.c" >}})

```shell
make thread_kill
./thread_kill
```
Source: [thread_kill.c]({{< github_url "thread_kill.c" >}})

```shell
make thread_sigsuspend
./thread_sigsuspend
```
Source: [thread_sigsuspend.c]({{< github_url "thread_sigsuspend.c" >}})
