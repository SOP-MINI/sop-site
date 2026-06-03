---
title: "Memory Management"
date: 2022-02-04T22:11:02+01:00
weight: 100
bookCollapseSection: true
---

# Zarządzanie Pamięcią

## Scope

- Physical address space
- Memory mapped devices
- System memory map
- Address translation
- Memory Management Unit
- Contiguous memory allocation
    - Algorithms: first-fit, best-fit, worst-fit
    - External fragmentation
- Segmentation
    - Segment table
    - Segment registers
    - Segmentation of 8086, 80286, GDT, LDT
    - Segmentation cons
- Paging
    - Frames and pages
    - Page tables
    - Internal fragmentation
    - TLB
    - Memory protection
    - Shared pages
    - Hierarchical page tables
    - Hashed page tables
    - Intel 32 and 64 bit

## Reference

1. [Lecture Slides](slides)
2. [Code samples](code)
3. Textbook:
    - chapter 8 - "Memory-management strategies".
    - chapter 9 - "Virtual-Memory Management",
4. Slides: [VM\_en.pdf]({{< resource "VM_en_4.pdf" >}})
5. Auxiliary reading (for those
   interested): [Linux MM Documentation](https://linux-mm.org/LinuxMMDocumentation), [Understanding the Linux Virtual Memory Manager](http://ptgmedia.pearsoncmg.com/images/0131453483/downloads/gorman_book.pdf)
