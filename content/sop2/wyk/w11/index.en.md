---
title: "File systems"
date: 2022-02-04T22:17:05+01:00
weight: 110
---

# Lecture 11 - File systems

## Scope of the lecture

  - File, file control block, i-node, file, system, file system layers
  - Virtual file system (VFS): goals, implementation, extensibility. FUSE
  - Directory implementations: linear list, array, hash table, tree (e.g. B+tree)
  - Allocation methods: contiguous, linked, indexed. Modifications: extents, FAT, multiple-level indexes.
  - Free-space management: bit vector or bit map, linked list of free blocks,space maps (ZFS)
  - Efficiency and performance. Optimization of sequential access: free-behind, read-ahead.
  - Page caching and I/O buffering. Pros and cons of unified caching.
  - Log structured (journaling) file systems.

## Reference

1.  Textbook: ch. 11 - "Implementing File Systems"
2.  Slides: [FS\_1en.pdf]({{< resource "FS_1en_3.pdf" >}})
