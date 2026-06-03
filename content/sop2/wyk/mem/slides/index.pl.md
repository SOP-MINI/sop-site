---
title: "Slides"
type: presentation
layout: single
---

# Memory Management

Operating Systems 2 Lecture

<small>Warsaw University of Technology<br/>Faculty of Mathematics and Information Science</small>

---

### Physical Address Space

Set of valid **physical** addresses usable by a program.
Usually different from logical one thanks to **the address translation** performed by the system.

![pas.svg](/ops2/wyk/mem/pas.svg)

---

### System memory map

The OS manages physical address space (PAS). It must determine where to put each program and where to put itself.
One could imagine PAS as large contiguous block of memory ranging from 0 to the size of the installed RAM chip.
However, the reality is far more compex (see `/proc/iomem`).

```text
00000000-00000fff : Reserved
00001000-0009efff : System RAM
0009f000-000fffff : Reserved
  000f0000-000fffff : System ROM
00100000-09afffff : System RAM
09b00000-09dfffff : ACPI Non-volatile Storage
...
74efe000-750fdfff : ACPI Non-volatile Storage
  750e5000-750e5fff : USBC000:00
750fe000-751fdfff : ACPI Tables
...
80000000-dfffffff : PCI Bus 0000:00
  80000000-97ffffff : PCI Bus 0000:61
  98000000-afffffff : PCI Bus 0000:01
```

---

### Memory mapped IO

Why the address space is so scattered? We have to see how hardware has evolved.

Processors like to map devices into the address space to save space on the PBC.
In early Intel processor - 8086 - everything was attached to the same 20 bit memory bus:

![8086_cs.svg](/ops2/wyk/mem/8086_cs.svg)

---

### Intel 8086 memory map

Back then, processor could address at most 1MB, partly occupied by devices. The device choice is made based on 4 highest
address bits.
Seemingly contiguous 1MB (`0x00000` - `0xFFFFF`) has different functional blocks:

![8086_cs.svg](/ops2/wyk/mem/8086_map.svg)

---

### North Bridge Era

Up to 2008 CPUs had single memory bus (FSB) attached to the North Bridge which routed
the request based on the address.

![fsb_arch.svg](/ops2/wyk/mem/fsb_arch.svg)

---

### 32b Address Space

The North Bridge was configured with **Top of Low Usable DRAM (TOULD)** register
determining the cutoff point between RAM addresses and devices address space.

![fsb_map.svg](/ops2/wyk/mem/fsb_map.svg)

If CPU accessed above TOULD value brige routed the request either to directly attached
AGP/PCIe video device or (fallback) to the South Bridge with slow devices. It also
emulated old ROM gap within the first 1MB.

---

### Modern SoC architecture

The North Bridge was a bottleneck and got embedded within the CPU itself.
It nowadays has internal logic block (i.e. Intel System Agent)
which monitors the address bus and routes the request appropriately.

![sa_arch.svg](/ops2/wyk/mem/sa_arch.svg)

---

### Modern system memory map

Modern address space (`/proc/iomem`) is deeply scattered thanks to backward compatibility.
UEFI passes the system memory map to the kernel early on.

![sa_map.svg](/ops2/wyk/mem/sa_map.svg)

---

### Address Translation

Within scattered block of _System RAM_ kernel must place itself and all the user tasks.
Tasks should not be able to access memory of other tasks, nor memory of the kernel.
Also, it should not matter where tasks are physically placed.

![sa_map.svg](/ops2/wyk/mem/mmu.svg)

CPU embeds a Memory Management Unit (MMU) which translates logical addresses emitted by tasks
into physical addresses. Kernel controls the translation proces by MMU registers.

---

### Contiguous Allocation

The simplest approach would be to use _contiguous allocation_: each task gets its own chunk of physical RAM.
MMU construction is simple in such case.

![contiguous_mmu.svg](/ops2/wyk/mem/contiguous_mmu.svg)

---

### Choosing the right place

Tasks appear and die dynamically leaving _holes of various sizes_. OS allocator picks a hole in some way, usually:
_first-fit_, _best-fit_ or _worst-fit_. The worst fit performs the worst obviously, first and best behave the same.

![contiguous_fit.svg](/ops2/wyk/mem/contiguous_fit.svg)

Kernel keeps information about each task residence in its task description structures. It iterates over the TCB list
searching for a suitable hole.

---

### The fragmentation problem

Over time, physical address space gets **fragmented**. Tasks leave large set of scattered, non-contiguous holes
which cannot be utilized. System becomes unstable due to **external fragmentation**.

System with 500KB loads sequentially programs A, B and C of sizes 100KB, 200KB, and 100KB.
Taks B ends, user now wants to load taks D of size 250KB.

![contiguous_frag.svg](/ops2/wyk/mem/contiguous_frag.svg)

This approach was used by the old systems from 60's and 70's like DEC PDP-10.

---

### Segmentation

Later approaches treated logical address space as a set of independent logical chunks called **segments**.

![segmentation.svg](/ops2/wyk/mem/segmentation.svg)

---

### Segmentating MMU operation

System maintains information about segment layout of each task within **segment table**.
Logical address is a tuple of segment and offset within the segment. MMU verifies the offset
and adds segment base to calculate physical address.

![segmentation_mmu.svg](/ops2/wyk/mem/segmentation_mmu.svg)

---

### The 8086 segmentation

Intel 8086 was 16-bit processor (ALU, registers) with 20-bit address bus.
To access more than 64K memory, the core used dedicated **segment registers**:

* `CS` (Code Segment)
* `DS` (Data Segment)
* `SS` (Stack Segment)
* `ES` (Extra Segment)

Core translated logical 16-bit address using segment offset:

`phys_addr = (segment_reg << 4) + logical_addr`

Instruction determined which segment to use.

This allowed **easy program relocation** and extended the addressable space but gave **no protection** at all.

---

### Intel 80286 segmentation

Newer processor introduced **protected mode**. Segment registers (CS, DS, SS) instead of simple offsets
became **selectors** - indices into **Global Descriptor Table** or **Local Descriptor Table**.

Segment selectors were 16-bits and contain segment index (13 bits), GDT/LDT flag (1 bit) and Requested
Privilege Level (2 bits). RPL of Code Segment (CS) determined executing program privileges (called CPL).

LDT described process-private segments, GDT - the global ones. The OS switched just the
LDT pointer during context switch (LDTP). Entries in both tables are 8 byte values and contained
segment offset and limit, Descriptor Privilege Level (DPL), allowed operations (read, write, execute).

The processor was inherently still 16-bit machine. A program must manipulate segment registers to address more than
64K memory.
Stores to segment register underwent verification based on RPL, CPL and DTL.

---

![80286_segmentation.svg](/ops2/wyk/mem/80286_segmentation.svg)


---

### 80286 segmentation flaws

- **64kB limit per contiguous allocation**: no larger arrays
- **Segment switch overhead**: segment changing instructions like `MOV DS, AX` must perform complex validations
- **Portability Issues**: segmentation was implemented differently elsewhere or nonexistent
- **External Fragmentation**: variable length segments still suffered from same fragmentation issues as contiguous
  approach
- **Complex Memory Model**: programs must be aware of segmented memory and access memory differently if the access needs adjusting the segment register

```c++
int val = 999;
int near *local_ptr = &val;
char far *vga_buffer = (char far *)MK_FP(0xB800, 0x0000);
```

---

### Paging

The final approach drops varying size chunks. Instead, 
it divides physical address space into **fixed-size frames** (i.e. 4kB).

Logical address space is similarly divided into fixed size **pages**.

Logical address is therefore composed of two parts: page number and
offset within the page. Address translation is touching offset at all.
It only maps page number into a frame number.

Paging incurs **internal fragmentation**. Some pages may be not utilized fully.

---

![paging.svg](/ops2/wyk/mem/paging.svg)

---

### Page Tables

To perform translation MMU uses an OS-maintained per-process **page table**.
Page index is treated as a table index, cell contains frame index.

![paging_translation.svg](/ops2/wyk/mem/paging_translation.svg)

Page table entries may be marked as invalid. Accessing beyond bage table limit,
or to the invalid page results in CPU exception.

---

### Translation Lookaside Buffer

Naively implemented paging incurs 2x memory access slowdown due to page table lookup.
To mitigate, the MMU embeds fast associative memory called **TLB**, caching mapping entries.

![paging_tlb.svg](/ops2/wyk/mem/paging_tlb.svg)

---

### Memory Protection

Page table entries (as well as TLB entries) frequently contain page attributes.

- **Valid bit**: if not set then page is not part of the logical address space.
Accessing logical address within this page causes MMU to generate CPU exception. 

- **Readonly bit**: prevents writes to a page.

- **Executable bit**: allows fetching instructions from a page.

Using them greatly enhances security properties of the system. For instance, stack
region is frequently mapped with non-executable pages eliminating whole class of buffer overflow attacks.

---

### Page Table Data Structures

Consider 32-bit system using 4KB pages. Each process may have _2^32 / 2^12 = 2^20_ page table entries, usually sparsely
utilized. Most pages are typically invalid (empty entries), unless process indeed uses gigabytes of memory.

Each entry has at least 4 bytes in size. Thus page table of each process, stored contiguously in kernel memory,
would take at least 4MB.

Kernel needs a better dictionary data structure than simple array to efficiently store page tables for processes
having varying memory footprints.

---

### Hierarchical Page Tables

Common approach is to implement the dictionary as a search tree. Nodes are tables. Upper level tables point to lower level tables.
Leaf tables point to frames.

![paging_hier.svg](/ops2/wyk/mem/paging_hier.svg)

---

### Hashed Page Tables

For large address spaces the dictionary might be implemented as a hash table.
- page number is hashed, the output is the page table index
- each table entry is a linked list, each node contains page and frame numbers
- during translation MMU needs to follow the chain until it finds a match

![paging_hash.svg](/ops2/wyk/mem/paging_hash.svg)


---

### Intel 32 bit paging

32-bit intel uses 2-level paging. `CR3` register points to top-level page table of a process called _Page Directory_.
10 most significant address bits are directory index.

![paging_32b_hierarchy.svg](/ops2/wyk/mem/paging_32b_hierarchy.svg)

---

### Intel 64 bit paging

In case of enormous 64-bit address space (>16EB!), far larger split is necessary. 

4-level paging is introduced. Only 48 address bits are used (still large enough). 

![paging_64bit.svg](/ops2/wyk/mem/paging_64bit.svg)
