---
title: "Filesystem"
date: 2022-02-05T22:35:32+01:00
weight: 20
---

# File system interface and stream-based I/O

## Scope

- File concept. File structures, attributes and basic operations.
- Opening file. File session. File locking.
- File access modes: sequential, direct (random access), indexed.
- File system organization: partitions, directories, formatting
- Directory organization: goals and implementations: single-level, 2-level tree-structured, acyclic-graph structured and
  generic graphs.
- File system mounting.
- File sharing: concept, semantics, protection.

Low-level POSIX API:

- Opening and closing file session: `open()`, `close()`
- File attributes in Unix and attribute reading functions: `stat(), lstat(),fstat()`
- Descriptor-based synchronous read/write: `read(), write()`
- File positioning: `lseek()`
- Descriptor duplicates: `dup(), dup2()`
- Changes in arrays of: descriptors, open files and i-nodes raled to calling `open(), dup()/dup2(), fork()`
- Waiting on descriptors: `select()`
- File streams and descriptors: `fdopen(), fileno()`
- Synchronized I/O: `fsync(), sync()`

Standard C API:

- Streams and descriptor-based I/O.
- Stream buffering.
- Opening/closing streams. Stream EOF and error condition indications
- Stream positioning and read/write operations.
- POSIX directories: concept, working directory, root directory.
- Directory stream related operations.

## Reference

1. Textbook: "File system interface - chapter 11"
2. [Lecture presentation](https://link.excalidraw.com/p/readonly/g9ICNTqqCX154vDBmccC) ([PDF]({{< resource OPS1_Filesystem_API.pdf >}}))
3. [Lecture code samples](https://gitlab.com/SaQQ/sop1/-/tree/master/02_filesystem2?ref_type=heads)
4. The GNU C library documentation: [Input/output concepts](https://www.gnu.org/software/libc/manual/html_node/I_002fO-Concepts.html#I_002fO-Concepts)
5. The GNU C library documentation: [Low-Level Input/Output (13.1-13.5,13.8)](https://www.gnu.org/software/libc/manual/html_node/#toc-Low_002dLevel-Input_002fOutput)
6. Excerpts from POSIX IEEE Std 1003.1 2017 - [POSIX definitions]({{< resource "POSIX_excerpts.pdf" >}}) related to
   processes
7. Old Slides: [FS\_interface.pdf]({{< resource "FS_interface_0.pdf" >}}), [IO\_1.pdf]({{< resource "IO_1_15.pdf" >}}), Slides: [IO\_2.pdf]({{< resource "IO_2_4.pdf" >}})
