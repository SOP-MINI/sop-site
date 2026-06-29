---
title: "Slides"
type: presentation
layout: single
---

# Virtual Memory

Operating Systems 2 Lecture

<small>Warsaw University of Technology<br/>Faculty of Mathematics and Information Science</small>

---

### Virtual memory description

Commonly, each process's control block contains two memory-related containers:

- virtual memory areas description - set of valid address ranges
- a page table - hierarchical page-frame dictionary (HW dependent structure)

![vmarea-generic.svg](/ops2/wyk/vmem/vmarea-generic.svg)

---

### Linux task struct attributes

Linux PCBs contain instance of [
`struct mm_struct`](https://elixir.bootlin.com/linux/v7.0.10/source/include/linux/mm_types.h#L1123).
Inside, one can find [
`struct vm_area_struct`'s](https://elixir.bootlin.com/linux/v7.0.10/source/include/linux/mm_types.h#L913) in a tree-like
structure
as well as root page directory pointer.

![vmarea-linux.svg](/ops2/wyk/vmem/vmarea-linux.svg)

VM area flags are mainly `mmap()` arguments (`VM_READ/VM_WRITE/VM_EXEC`) combined with
more advanced flags supporting distinct features like `VM_LOCKED`, `VM_DONTCOPY`, `VM_DONTDUMP`, `VM_NORESERVE`.

---

### Why Virtual not Logical?

Programs usually do not utilize all their memory all the time. Usually program works locally
within some area (_working set_) and then moves to the other one. Keeping memory allocated
statically is therefore wasteful.

To achieve better hardware utilization OS'es implement **Virtual Memory** mechanisms.
Virtual address may or may not refer to a physical location. Only when the memory is actually needed - process reads or
writes from/to a page - physical frame is allocated.

This is usually implemented through **on-demand paging** with clever handling of MMU **Page Fault** exceptions.
On demand paging supports **lazy allocation** of physical memory as well as **swapping out** unused areas.

---

### X86_64 Page Table Entries

Take 64-bit Intel as an example. It defines the following layout of 8-byte leaf PTEs:

![x86_64_pte.svg](/ops2/wyk/vmem/x86_64_pte.svg)

Each page table entry therefore specifies if page is executabe/writable, if it's **present**
in RAM, can it be accessed by unprivileged code (user), was it read (accessed) or written (dirty).

---

### Lazy allocation (1)

Consider process having a single VM area.

![ondemand_1.svg](/ops2/wyk/vmem/ondemand_1.svg)

It needs more memory...

---

### Lazy allocation (2)

... so it issues `mmap()` call. The syscall picks empty VM region, constructs new VM entry and returns
without modifying the Page Table!

![ondemand_2.svg](/ops2/wyk/vmem/ondemand_2.svg)

---

### Lazy allocation (3)

Later process writes to the newly obtained memory area. MMU attempts address translation.
**Page table walk** results in error - frame not present. MMU triggers **Page Fault** -  a CPU exception.

![ondemand_3.svg](/ops2/wyk/vmem/ondemand_3.svg)

Control is passed to the OS installed exception handling code.

---

### Lazy allocation (4)

Page fault handler checks running task's VM areas, and confirms the address is correct.
It picks an empty frame, updates taks's page table and schedules it for resumption.
As soon as process is resumed, the retried instruction executes without an exception.

![ondemand_4.svg](/ops2/wyk/vmem/ondemand_4.svg)

Process continues unaware of anything that happened in the meantime.

---

### Virtual memory performance

In case page fault occurs an extremely time-consuming process must happen:
- switch to privileged mode, saving context, process suspension
- VM areas lookup inside TCB
- search for a free frame (possibly evicting another's process memory!)
- modification of process's page map
- process resumption
- eventually process gets dispatched and retries memory access

From the process's point of view it is as if single memory access took ages!

Common programing technique mitigating this is **page pre-faulting**, preventing later latency issues.

---

### Process memory footprint

System supporting Virtual Memory measures memory consumption in various ways:
- `VIRT` - total virtual address space, not necessarily physically allocated
- `RES`/`RSS` (Resident Set Size) - total size of physically mapped memory frames

Pages may be shared between processes. In such case `RSS` of many processes includes
same physical region. Thus, more metrics are needed:

- `SHR`/`SHARED` - total size of physically mapped shared memory frames
- `PSS` (Proportional Set Size) - an adjusted metric combining private memory and a proportional share of shared
  memory (e.g., a 12MB shared library used by 4 processes adds 3MB to each process's PSS).
- `USS` (Unique Set Size) - unshared, private memory specific to this process.

`VIRT` usage tells nothing about real memory usage!

---

### Memory Overcommit

By default, the Linux kernel promises more memory to processes than is physically available,
assuming not all processes will use their allocated VIRT memory simultaneously.
This behavior is controlled by `vm.overcommit_memory`:

```shell
cat /proc/sys/vm/overcommit_memory
```

Mode `0`: Heuristic Overcommit (Default)

Heuristic: _Small_ overcommits are allowed; absurdly large requests are denied.

Mode `1`: Always Overcommit

The kernel blindly approves all memory requests, ignoring current memory usage.

Mode `2`: Strict No-Overcommit

The kernel strictly denies any allocation if<br/>VM usage > Swap + (RAM * `vm.overcommit_ratio`).

---

### OOM Killer

When system runs short on memory the system must act brutally and kill memory intensive tasks.
It picks a victim based on heuristic algorithm.

It calculates `oom_score` (0-1000) for each process, roughly proportional to its memory usage.
The higher, the riskier:

```c++
long get_oom_score(Process p) {
  long p = p.rss_pages + p.swap_pages + p.page_table_pages;
  long total = sys.total_ram_pages + sys.total_swap_pages;
  long score = (p * 1000) / total;
  if (has_capability(p, CAP_SYS_ADMIN)) {
      score -= 30;
  }
  score += p.oom_score_adj;
  if (score < 0) return 0;
  if (score > 1000) return 1000;
  return score;
}
```

---

### Page reclaim

When physical RAM is exhausted and a new frame is needed (e.g., during a Page Fault),
the OS attempts to **evict** an existing page. This is called **Page Reclaim**.

**Anonymous pages**

_e.g., heap, stack_

These pages have no filesystem backing.
To free their frames, the OS writes them to a dedicated disk area called **Swap space** (partition or file).

**File-backed pages**

_e.g., code, memory-mapped files_

If unmodified (**clean**), they are simply discarded.
If modified (**dirty**), they are synced back to the filesystem.

Swapping extends the apparent physical memory at the cost of disk I/O latency.

---

Kernel reclamation process picks a victim frame to swap out.
Victim process gets suspended to prevent memory accesses.

![swap_1.svg](/ops2/wyk/vmem/swap_1.svg)

---

Page table is modified to store swap location.
Next attempt to access swapped out page will result in a page fault and I/O transfer back.

![swap_2.svg](/ops2/wyk/vmem/swap_2.svg)

---

### When reclamation happens?

The OS tries to keep configurable minimal amount of free frames. It defines 3 _watermarks_:
- `min`: absolute reserve, hitting it triggers **direct reclamation** mode
- `low = 1.25 * min`: going below it activates **asynchronous reclamation** process (`kswapd`) attempting to free memory up to `high`
- `high = 1.5 * min`: above it async reclamation is disabled.

This hysteresis provides stability of enabling and disabling async process.

In direct mode, reclamation is synchronously executed in the context of the process which triggered a page fault,
slowing it down drastically.

---

### Reclamation watermarks

![watermarks.svg](/ops2/wyk/vmem/watermarks.svg)

---

### Local vs Global Replacement

When a page fault occurs, where does the OS find a victim frame?

- **Local Replacement**: The process can only select a victim from its own set of allocated frames. 
  - *Pros*: Predictable performance; one process cannot thrash another. 
  - *Cons*: Wastes memory if a process doesn't fully utilize its allocation.

- **Global Replacement**: A process can select a replacement frame from the set of all frames, even if it belongs to another process.
  - *Pros*: Better overall system throughput and memory utilization (used by Linux, Windows).
  - *Cons*: One memory-hogging process can impact the entire system.

---

### Local Replacement: Frame Allocation

If an OS uses Local Replacement, how many frames does each process get?

- **Fixed Allocation**: Every process gets $N$ frames.
- **Proportional Allocation**: Frames allocated based on process size.
- **Dynamic Allocation**: 
  - **Working-Set Model**: Tracks the set of actively used pages over a time window $\Delta$. Allocation grows/shrinks dynamically.
  - **Page-Fault Frequency (PFF)**: Establish acceptable upper and lower bounds on the fault rate. If the rate is too high, allocate more frames; if too low, remove frames.

---

### Global Replacement: Domino Effect

In Global Replacement, a single misbehaving process can steal frames from all other processes.

If Process A starts thrashing, it evicts pages belonging to Process B and C. 
Now B and C page fault, stealing frames back. The entire OS grinds to a halt!

**Solution**: Containment. Modern OSes use Resource Limits (e.g., **Linux cgroups**) to enforce a hard local limit on a globally-replaced system. This protects the rest of the OS.

---

### FIFO Algorithm Simulation

*(Note: The following algorithm simulations demonstrate a **Local Replacement Policy** with a fixed allocation of 3 frames)*

Evicts the oldest page. Suffers from *Belady's Anomaly*.

| Request | 7 | 0 | 1 | 2 | 0 | 3 | 0 | 4 | 2 | 3 |
|---------|---|---|---|---|---|---|---|---|---|---|
| Frame 1 | 7 | 7 | 7 | 2 | 2 | 2 | 2 | 4 | 4 | 4 |
| Frame 2 |   | 0 | 0 | 0 | 0 | 3 | 3 | 3 | 2 | 2 |
| Frame 3 |   |   | 1 | 1 | 1 | 1 | 0 | 0 | 0 | 3 |
| Fault?  | F | F | F | F |   | F | F | F | F | F |

Total faults: 9 (on 3 frames).

---

### OPT Algorithm Simulation

Evicts the page that will not be used for the longest time in the future.

| Request | 7 | 0 | 1 | 2 | 0 | 3 | 0 | 4 | 2 | 3 |
|---------|---|---|---|---|---|---|---|---|---|---|
| Frame 1 | 7 | 7 | 7 | 2 | 2 | 2 | 2 | 2 | 2 | 2 |
| Frame 2 |   | 0 | 0 | 0 | 0 | 0 | 0 | 4 | 4 | 3 |
| Frame 3 |   |   | 1 | 1 | 1 | 3 | 3 | 3 | 3 | 3 |
| Fault?  | F | F | F | F |   | F |   | F |   | F |

Total faults: 7. Optimal, but impossible to implement.

---

### LRU Algorithm Simulation

Evicts the page that has not been accessed for the longest time in the past.

| Request | 7 | 0 | 1 | 2 | 0 | 3 | 0 | 4 | 2 | 3 |
|---------|---|---|---|---|---|---|---|---|---|---|
| Frame 1 | 7 | 7 | 7 | 2 | 2 | 2 | 2 | 4 | 4 | 4 |
| Frame 2 |   | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 3 |
| Frame 3 |   |   | 1 | 1 | 1 | 3 | 3 | 3 | 2 | 2 |
| Fault?  | F | F | F | F |   | F |   | F | F | F |

Total faults: 8. Good approximation of OPT, but hard to implement in HW.

---

### Clock Algorithm (Second-Chance)

Approximates LRU using an **Accessed** bit (set to `1` by MMU on access, cleared to `0` by OS).
Frames are in a circular list. OS scans for a page with bit `0`. If bit is `1`, it is cleared to `0` (second chance) and the pointer moves on.

| Request | 7 | 0 | 1 | 2 | 0 | 3 | 0 | 4 | 2 | 3 |
|---------|---|---|---|---|---|---|---|---|---|---|
| F1 (A)  | 7(1)| 7(1)| 7(1)| 2(1)| 2(1)| 2(1)| 2(1)| 4(1)| 4(1)| 4(1)|
| F2 (A)  |     | 0(1)| 0(1)| 0(0)| 0(1)| 0(0)| 0(1)| 0(0)| 2(1)| 2(1)|
| F3 (A)  |     |     | 1(1)| 1(0)| 1(0)| 3(1)| 3(1)| 3(0)| 3(0)| 3(1)|
| Fault?  | F   | F   | F   | F   |     | F   |     | F   | F   | F   |

*(F1, F2, F3 indicate frames; (A) is the Accessed bit state after the request)*

---

### Dirty Bit in Eviction

The OS doesn't only look at the Accessed bit. The PTE also contains a **Dirty (Modified)** bit.

- **Clean Page** `(Dirty=0)`: Evicting it is fast. The OS just discards it.
- **Dirty Page** `(Dirty=1)`: Evicting it is slow. The OS must write it to disk (Swap or file) before reusing the frame.

Therefore, eviction algorithms prefer pages with `(Accessed=0, Dirty=0)` over `(Accessed=0, Dirty=1)`.

---

### Linux LRU Approximation & Eviction

Linux maintains two main lists to approximate LRU:
- **Active List**: Pages recently accessed (MRU).
- **Inactive List**: Pages not accessed recently, candidates for eviction (LRU).

**How it ties to Watermarks**:
When free memory drops below the `low` watermark, the kernel wakes up the `kswapd` daemon.
- `kswapd` scans the tail of the **Inactive List** to find victims.
- It shrinks the Inactive List by reclaiming clean pages and swapping out dirty pages.
- To replenish the Inactive List, it demotes pages from the tail of the **Active List**.
- If an inactive page is accessed again, it gets promoted back to the head of the Active List.
- `kswapd` stops when free memory reaches the `high` watermark.

---

### Thrashing

If the sum of **Working Sets** (the set of pages actively used) of all running processes exceeds the total physical memory allocation, the system will constantly trigger page faults.
When the OS swaps out a page from one process to bring in a page for another, the swapped-out page is needed again almost immediately.

This state is called **Thrashing**.
- Disk I/O becomes the bottleneck.
- CPU utilization plummets because processes are mostly blocked waiting for pages.
- The OS might mistakenly think the CPU is idle and admit *more* processes, worsening the problem!