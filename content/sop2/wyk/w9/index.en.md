---
title: "Lec9. Memory management"
date: 2022-02-04T22:05:56+01:00
draft: false
---

## Scope of the lecture

  - Binding of instructions and data to memory:
      - compilation
      - linking: static, dynamic, dynamic during execution
      - loading: absolute, on demand, dynamic
      - execution; overlaying, swapping.
  - Logical vs. physical address space. Memory Management Unit (MMU).
  - Memory allocation techniques:
      - Contiguous allocation, dynamic allocation
      - Segmentation: architecture, properties
      - Paging: architecture, properties. Implementation of page table, associative memory/translation look-aside buffers (TLB) and effective memory access time. Hierarchical page tables. Hashed page table Inverted page table.
  - Memory fragmentation: internal and external.
  - Shared memory in segmentation and paging.

## Reference

1.  Textbook: chapter 8 - "Memory-management strategies".
2.  Slides: [Mem\_en.pdf]({{< resource "Mem_en_1.pdf" >}})
