---
title: "Realizacje systemów plików"
date: 2022-02-05T15:56:44+01:00
weight: 120
---

# Wykład 12 - Realizacje systemów plików, rozproszone systemy plików

## Zakres wykładu

  - Realizacje systemów plików
      - Systemy plików w SO Linux: ext2, ext3, ext4, proc, swap, ZFS
      - MSWwin - FAT12/16/32, exFAT, NTFS
      - Zarządzanie dyskami: partycjonowanie, formatowanie, rozruch PC (BIOS/MBR, UEFI/GPT).
  - Rozproszone systemy plików (*Distributed File Systems, DFS*) - treść opcjonalna.
      - Podstawowe koncepcje: system rozproszony, rozproszony system plików, klient, serwer, usługa, przezroczystość.
      - Struktura DFS
      - Struktury nazewnicze: przezroczystość położenia (*location transparency*), niezależnosć położenia (*location independence*)
      - 3 podstawowe podejścia do schematów nazewniczych
      - Zdalny dostęp do pliku (*remote file access*): mechanizm usługi zdalnej (*remote-service*) i pamięć podręczna (caching: in disk/main memory). Problem spójności pamięci podręcznej. Polityka uaktualniania pamięci podręcznej. Spojność plików i jej weryfikacja.
      - Doglądana obsługa pliku (*stateful file service*), a niedoglądana obsługa pliku (*stateless file services*): zalety i wady.
      - Zwielokrotnianie plików. Deduplikacja plików. Obszary zastosowań.
      - Network File System (NFS)

## Materiały

1.  Slajdy: [DFS.pdf]({{< resource "DFS_0.pdf" >}}), [FS\_2.pdf]({{< resource "FS_2_7.pdf" >}})
2.  Podręcznik: rozdz. 16 - "Rozproszone systemy plików", podrozdz. 20.8 - "Wejście i wyjście"
3.  `man: acl(5), filesystems(5), ext3(5), ntfs-3g(8),  proc(5), zfs(8), df(1), du(1), find(1), mkswap(5), stat(1)`
