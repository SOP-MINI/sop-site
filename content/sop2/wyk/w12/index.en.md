---
title: "Lec12. Implementations of file systems. Distributed file systems."
date: 2022-02-05T15:56:46+01:00
---
## Scope of the lecture

  - Implementations of file systems
      - Linux file systems: ext2, ext3, ext4, proc, swap, ZFS
      - MSWwin - FAT12/16/32, exFAT, NTFS
      - Disk management: partitioning, formatting, booting (BIOS/MBR, UEFI/GPT)
  - Distributed File Systems - optional content.
      - Basic concepts: distributed system, distributed file system (DFS), client, server, service, transparency.
      - DFS structure
      - Naming structures: Location transparency, Location independence
      - 3 basic naming schemes
      - Remote file access: remote-service and caching (disk/main memory). Consistency. Cache update policy.
      - Stateful vs stateless file services: pros and cons
      - File replication. Data deduplication. Areas of application.
      - Network File System (NFS)

## Reference

1.  Slides: [FS\_2en.pdf]({{< resource "FS_2en_7.pdf" >}}), [DFS\_en.pdf]({{< resource "DFS_en_1.pdf" >}})
2.  Textbook: chapter 11.8: NFS, chapter 12.5: Disk Management, chapter 12.6: Swap Space Management, Chapter 12.7: RAID structure, chapter 17: Distributed File Systems, chapter 21 - The Linux System, sec. 21.7 - "File Systems", chapter 22 - "Windows XP", sec. 22.5 - "File System".
3.  `man: acl(5), filesystems(5), ext3(5), ntfs-3g(8),  proc(5), zfs(8), df(1), du(1), find(1), mkswap(5), stat(1)`

