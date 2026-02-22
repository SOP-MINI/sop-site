---
title: "Shared memory"
date: 2022-02-10T10:00:00+01:00
weight: 10
---

# Shared memory

## Scope

- Mapping files into process memory (`mmap()`, `munmap()`).
- Mapping visibility flags (`MAP_SHARED`, `MAP_PRIVATE`, `MAP_ANONYMOUS`).
- Memory protection flags (`PROT_READ`, `PROT_WRITE`, `PROT_EXEC`, `PROT_NONE`).
- Inter-Process Communication (IPC) via memory-mapped files.
- Copy-on-Write mechanism in private mappings.
- Disk synchronization (`msync()`, `MS_SYNC`, `MS_ASYNC`).
- Process address space structure.
- Debugging memory mappings on Linux (`/proc/<pid>/maps`).
- Mapping inheritance and behavior on `fork()`.
- POSIX Shared Memory object lifecycle (`shm_open()`, `ftruncate()`, `shm_unlink()`).
- SHM namespace (`/dev/shm/`).
- Anonymous mappings for IPC between related processes.

## Reference

1. [Lecture Slides]({{< ref "slides" >}})
2. Supplementary reading: W.R. Stevens, Unix Network Programming, Vol. 2, 2nd Ed.: Interprocess Communications,
   Chapters: 
   - 12 (Shared Memory Introduction)
   - 13 (Posix Shared Memory)
   - 14 (System V Shared Memory)
