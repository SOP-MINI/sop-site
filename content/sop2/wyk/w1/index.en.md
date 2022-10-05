---
title: "Lec1. Introduction to interprocess communication (IPC). IPC with pipes/FIFOs"
date: 2022-02-10T10:00:00+01:00
---
## Scope
  
### Basic content

  - Independent vs cooperating processes. Reasons for cooperation.
  - Two fundamental models of interprocess communication (IPC): shared memory, message passing.
  - Main features of shared memory based IPC.
  - Main features of message passing based IPC.
  - POSIX pipes/FIFOs
      - POSIX definition and semantics of pipes/FIFOs. Creation/destruction, file-like access without positioning capability, consequences of bounded buffering, blocking/non-blocking access.
      - (anonymous) pipes - creation: `pipe()`, destruction: `close()` of all references. Pipe and `fork()` - opportunities. Example uses. Shortcut: `popen()`, `pclose()`
      - FIFOs ("named pipes") - creation: `mkpipe()`, desctruction: `unlink()` vs `close()` of all open descriptors related to FIFO.
      - Error condition notifications: `SIGPIPE` signal, `errno` values: `EPIPE`, `ESPIPE`.
      - Blocking mode FIFO access and possible deadlock.

### Auxiliary content

  - Non-blocking use of FIFOs: pros and cons.
  - Pipes in non-Posix systems (notably MS Win).

## Reference

1.  Textbook: chapter. 3.4 (Interprocess communication).
2.  Slides: [IPC\_intro.pdf]({{< resource "IPC_intro.pdf" >}}), [POSIX-pipes.pdf]({{< resource "POSIX-pipes.pdf" >}})
3.  Section 15 of the GNU C library documentation: [Pipes and FIFOs](http://www.gnu.org/software/libc/manual/html_node/Pipes-and-FIFOs.html#Pipes-and-FIFOs)
4.  Supplementary reading: Ch. 4 (Pipes and FIFOs) of the book: W.R. Stevens, Unix Network Progamming, Vol. 2, 2nd Ed.: Interprocess Communications
