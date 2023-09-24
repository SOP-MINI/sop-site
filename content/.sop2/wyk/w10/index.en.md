---
title: "Virtual memory"
date: 2022-02-04T22:11:02+01:00
weight: 100
---

# Lecture 10 - Virtual memory

## Scope of the lecture

  - Virtual memory - fundamentals:
      - Motivation: smaller memory allocation ⇒ better utilization, higher degree of multi-programming, smaller I/O -related overhead; virtual address space can be much larger than physical and sparse
      - Implementations: demand paging, demand segmentation
      - Memory management unit (MMU)
  - Demand paging:
      - valid-invalid bid, page fault, page swapping
      - lazy swapping, pager
      - memory reference locality and page fault frequency
      - CPU instruction restart
      - average performance of demand paging, average memory access time
      - page/frame replacement algorithms: FIFO, OPT, LRU and approximations; Belady effect
      - page buffering and raw disk mode
      - allocation of frames to a process: equal/proportional/priority, local/global; NUMA, CPU affinityS.
      - thrashing: causes, symptoms, remedies; working-set mode, page-fault frequency, middle-term scheduler.
  - Measures to improve the use of memory access:
      - memory-mapped files; memory-mapped I/O devices
      - copy-on-write(COW) and efficiency of `fork()`
      - allocating kernel memory; buddy algorithm, slab algorithm
      - locality of memory references and efficiency of large data structures processing
  - Examples of VM implementations: Windows, Unix, Linux

## Reference

1.  Textbook:
  - chapter 9 - "Virtual-Memory Management",
  - chapter 21 - "The Linux System", sec. 21.6.2: "Virtual memory",
  - chapter 22 - "Windows XP", sec. 22.3.3.2: "Virtual memory manager".
2.  Slides: [VM\_en.pdf]({{< resource "VM_en_4.pdf" >}})
3.  Auxiliary reading (for those interested): [Linux MM Documentation](https://linux-mm.org/LinuxMMDocumentation), [Understanding the Linux Virtual Memory Manager](http://ptgmedia.pearsoncmg.com/images/0131453483/downloads/gorman_book.pdf)
