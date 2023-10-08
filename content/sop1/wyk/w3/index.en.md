---
title: "Low-level I/O"
date: 2022-02-05T22:35:36+01:00
weight: 30
---

# Low-level input/output

## Scope

Low-level (descriptor-based) I/O in POSIX/UNIX

  - Opening and closing file session: `open()`, `close()`
  - File attributes in Unix and attribute reading functions: `stat(), lstat(),fstat()`
  - Descriptor-based synchronous read/write: `read(), write()`
  - File positioning: `lseek()`
  - Descriptor duplicates: `dup(), dup2()`
  - Changes in arrays of: descriptors, open files and i-nodes raled to calling `open(), dup()/dup2(), fork()`
  - Waiting on descriptors: `select()`
  - File streams and descriptors: `fdopen(), fileno()`
  - Synchronized I/O: `fsync(), sync()`

## Reference

1.  Slides: [IO\_2.pdf]({{< resource "IO_2_4.pdf" >}})
2.  Extra reading: The GNU C library documentation: [Low-Level Input/Output (13.1-13.5,13.8)](https://www.gnu.org/software/libc/manual/html_node/#toc-Low_002dLevel-Input_002fOutput)
3.  Exemplary lecture code: [list.c]({{< resource "list.c" >}}), [read\_dup.c]({{< resource "read_dup.c" >}}), [read\_fork.c]({{< resource "read_fork.c" >}}), [read2.c]({{< resource "read2.c" >}}), [unlnk.c]({{< resource "unlnk.c" >}})
