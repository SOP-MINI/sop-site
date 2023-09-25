---
title: "Lecture Test 2"
date: 2022-02-05T16:21:02+01:00
weight: 145
---

- OPS1.Lec8: [Management of threads and processes]({{< ref "../w8" >}})
- OPS2.Lec9: [Memory management]({{< ref "../w9" >}}).
- OPS2.Lec10: [Virtual memory]({{< ref "../w10" >}}).
- OPS2.Lec11: [File systems]({{< ref "../w11" >}}).
- OPS2.Lec12: [Implementations of file systems]({{< ref "../w12" >}}). \[The topic: "Distributed file systems" is beyond the scope of this test.\]
- OPS2.Lec13: [Real-time systems]({{< ref "../w13" >}}).
- OPS2.Lec14: [Protection and security]({{< ref "../w14" >}}).

Test can contain "calculation tasks"; they might be similar to the following ones. Note that a calculator is required, to perform calculations.

  - Assume a demand-paged memory with the page table held in fast associative registers. It takes ...seconds to service a page fault if an empty page is available or the replaced page is not modified, and ... seconds if the replaced page is modified. Memory access time is ... seconds. Assume that the page to be replaced is modified (on average) .... percent of the time. What is the maximum acceptable page-fault rate for effective access time of no more than .... seconds?
  - Given memory occupancy as shown in the table, specify how would each of the algorithms: a) first-fit (FF), best-fit (BF), worst-fit (WF), d) buddy allocator (BA), place the following sequence of memory requests: .... (in order)?
  - How many disk blocks have to be read-in, to perform the following: 1) opening access to a file `/a/file` in ext2 file system, and 2) reading 1MiB of its content? Assume, that disk clusters are of 1KiB size, index block takes one cluster, the catalogue `a` size is 4KiB, and cluster addresses have length 4B. Assume also, that the disk controller transmits (to RAM) content of one cluster a time, and that a copy of the main catalogue of the file system resides in kernel memory since the file system was mounted.
  - Consider the following page reference string: 1,2,3,... How many page faults would occur for the following replacement algorithms, assuming N frames (initially empty): a) LRU replacement, b) FIFO replacement, c) optimal replacement. Assume N=...
  - Suppose the system knows arrival times, priorities and CPU-phase lengths of processes (shown in a table). Draw *Gantt diagams* that illustrate scheduling with the algorithms detailed in OPS1.Lec8 (FIFO,SJF, SRJF, RR, priority). Calculate average process wait time (or turn-around time or maximum waiting time etc.) for each algorithm.
