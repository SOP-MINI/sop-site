---
title: "Signals"
date: 2022-02-05T22:35:41+01:00
weight: 50
---

# POSIX signals

## Scope

  - POSIX signal concept.
  - Signal sources: hardware exceptions, actions by processes
  - Signal targets: specific thread, process or process group.
  - Common UNIX signals
  - Actions to be taken in response to signals:
      - signal delivery:
          - signal ignoring
          - running user-defined handler
          - executing a system action (exit, core dump+exit, continue, stop)
      - signal acceptance: `sigwait()`
  - Programmatic signal generation: `kill()`, `alarm()`,...
  - Setting up user-defined signal handling: `sigaction()`
  - Signal mask concept and signal blocking: `sigprocmask()`, `sigpending()`
  - Actions to be taken in case of multiple occurences of signals.
  - `volatile sig_atomic_t` type and signal handling.
  - Possible side effects of asynchronous signal delivery:
      - premature exit of "long" system functions (with `errno==EINTR`) and `TEMP_FAILURE_RETRY` macro.
      - unpredicted behaviour of non-reentrant functions; async-safe functions.
  - Waiting for a signal: `pause()`, `sigsuspend()`, `sigwait()`.
  - Terminal generated signals. `stty`

## Reference

1. [Lecture presentation](https://link.excalidraw.com/p/readonly/awkHYH5frIwYCA09WJLg)
2. [Lecture code samples](https://gitlab.com/SaQQ/sop1/-/tree/master/04_signals?ref_type=heads)
3. Extra reading: The GNU C library documentation: [Signal Handling (24.1-24.8)](https://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html#Signal-Handling)
4. Old Slides: [POSIX\_signals.pdf]({{< resource "POSIX_signals_6.pdf" >}})
