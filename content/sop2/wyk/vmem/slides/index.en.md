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

### Victim frame choice

TBD