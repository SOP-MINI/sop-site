---
title: "Filesystem"
date: 2022-02-05T22:35:32+01:00
weight: 30
---

# Lecture 3 - File system interface and stream-based I/O

## Scope 

  - File concept. File structures, attributes and basic operations.
  - Opening file. File session. File locking.
  - File access modes: sequential, direct (random access), indexed.
  - File system organization: partitions, directories, formatting
  - Directory organization: goals and implementations: single-level, 2-level tree-structured, acyclic-graph structured and generic graphs.
  - File system mounting.
  - File sharing: concept, semantics, protection.
  - Streams and descriptor-based I/O.
  - Stream buffering.
  - Opening/closing streams. Stream EOF and error condition indications
  - Stream positioning and read/write operations.
  - POSIX directories: concept, working directory, root directory.
  - Directory stream related operations.

## Reference

1.  Textbook : chapter 11 "File system" - in 8th ed.; or chapter "File system interface - nr 11 in 9-th ed., nr 13 in 10th ed.
2.  Slides: [FS\_interface.pdf]({{< resource "FS_interface_0.pdf" >}}), [IO\_1.pdf]({{< resource "IO_1_15.pdf" >}})
3.  Extra reading: The GNU C library documentation: [Input/output concepts](https://www.gnu.org/software/libc/manual/html_node/I_002fO-Concepts.html#I_002fO-Concepts)
4.  Excerpts from POSIX IEEE Std 1003.1 2017 - [POSIX definitions]({{< resource "POSIX_excerpts.pdf" >}}) related to processes
